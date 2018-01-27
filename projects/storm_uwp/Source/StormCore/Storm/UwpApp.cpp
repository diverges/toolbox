#include "StormStd.h"
#include "UwpApp.h"

using namespace GameCore;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;


#pragma region Public Constructor

UwpApp::UwpApp() :
    m_windowClosed(false),
    m_windowVisible(true)
{}

#pragma endregion

#pragma region Application Methods
// First Method Called on IFrameworkView creation.
void UwpApp::Initialize(Windows::ApplicationModel::Core::CoreApplicationView ^ applicationView)
{
    //TODO: Registeractivation, suspend, and resume events.

    //TODO: Init Memory Leak Detection

    //TODO: Init Logger
}

// Called when the CoreWindow object is created
void UwpApp::SetWindow(Windows::UI::Core::CoreWindow ^ window)
{
    window->SizeChanged +=
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &UwpApp::OnWindowSizeChanged);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &UwpApp::OnVisibilityChanged);

    window->Closed +=
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &UwpApp::OnWindowClosed);

    DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

    currentDisplayInformation->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &UwpApp::OnDpiChanged);

    currentDisplayInformation->OrientationChanged +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &UwpApp::OnOrientationChanged);

    DisplayInformation::DisplayContentsInvalidated +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &UwpApp::OnDisplayContentsInvalidated);
}

// Initialize scene resources.
void UwpApp::Load(Platform::String ^ entryPoint)
{
    throw ref new Platform::NotImplementedException();

    //TODO: Init app and start timer.
}

// Called after window becomes active.
void UwpApp::Run()
{
    throw ref new Platform::NotImplementedException();

    //TODO: run loop
    //TODO: handle DX debugger queue

    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            auto commandQueue = GetDeviceResources()->GetCommandQueue();
            PIXBeginEvent(commandQueue, 0, L"Update");
            {
                m_main->Update();
            }
            PIXEndEvent(commandQueue);

            PIXBeginEvent(commandQueue, 0, L"Render");
            {
                if (m_main->Render())
                {
                    GetDeviceResources()->Present();
                }
            }
            PIXEndEvent(commandQueue);
        }
        else
        {
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void UwpApp::Uninitialize()
{}
#pragma endregion

#pragma region Application Event Handlers

void UwpApp::OnActivated(Windows::ApplicationModel::Core::CoreApplicationView ^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs ^ args)
{
    // Run() won't start until the CoreWindow is activated.
    CoreWindow::GetForCurrentThread()->Activate();
}

void UwpApp::OnSuspending(Platform::Object ^ sender, Windows::ApplicationModel::SuspendingEventArgs ^ args)
{
    // Save app state asynchronously after requesting a deferral. Holding a deferral
    // indicates that the application is busy performing suspending operations. Be
    // aware that a deferral may not be held indefinitely. After about five seconds,
    // the app will be forced to exit.
    SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

    //create_task([this, deferral]()
    //{
    //    m_main->OnSuspending();
    //    deferral->Complete();
    //});
}

void UwpApp::OnResuming(Platform::Object ^ sender, Platform::Object ^ args)
{
    // Restore any data or state that was unloaded on suspend. By default, data
    // and state are persisted when resuming from suspend. Note that this event
    // does not occur if the app was previously terminated.

    //m_main->OnResuming();
}
#pragma endregion

#pragma region Window Event Handlers

void UwpApp::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
    GetDeviceResources()->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
    m_main->OnWindowSizeChanged();
}

void UwpApp::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
    m_windowVisible = args->Visible;
}

void UwpApp::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
    m_windowClosed = true;
}

#pragma endregion

#pragma region DisplayInformation
void UwpApp::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
    // Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
    // if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
    // you should always retrieve it using the GetDpi method.
    // See DeviceResources.cpp for more details.
    GetDeviceResources()->SetDpi(sender->LogicalDpi);
    m_main->OnWindowSizeChanged();
}

void UwpApp::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
    GetDeviceResources()->SetCurrentOrientation(sender->CurrentOrientation);
    m_main->OnWindowSizeChanged();
}

void UwpApp::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
    GetDeviceResources()->ValidateDevice();
}

std::shared_ptr<DX::DeviceResources> UwpApp::GetDeviceResources()
{
    if (m_deviceResources != nullptr && m_deviceResources->IsDeviceRemoved())
    {
        // All references to the existing D3D device must be released before a new device
        // can be created.

        m_deviceResources = nullptr;
        m_main->OnDeviceRemoved();

#if defined(_DEBUG)
        ComPtr<IDXGIDebug1> dxgiDebug;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
        {
            dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
        }
#endif
    }

    if (m_deviceResources == nullptr)
    {
        m_deviceResources = std::make_shared<DX::DeviceResources>();
        m_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
        m_main->CreateRenderers(m_deviceResources);
    }
    return m_deviceResources;
}
#pragma endregion

