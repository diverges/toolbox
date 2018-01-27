#pragma once

#include "Common\DeviceResources.h"

namespace GameCore
{
    ref class UwpApp sealed : public Windows::ApplicationModel::Core::IFrameworkView
    {
    public:
        UwpApp();

        // IFrameworkView methods.
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
        virtual void Load(Platform::String^ entryPoint);
        virtual void Run();
        virtual void Uninitialize();

    protected:
        //// Application lifecycle event handlers.
        void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
        void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
        void OnResuming(Platform::Object^ sender, Platform::Object^ args);

        // Window event handlers.
        void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
        void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

        // DisplayInformation event handlers.
        void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
        void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
        void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

    private:
        bool m_windowClosed;
        bool m_windowVisible;

        std::shared_ptr<DX::DeviceResources> GetDeviceResources();
        std::shared_ptr<DX::DeviceResources> m_deviceResources;
    };

    ref class ApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
    {
    public:
        virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView() { return ref new UwpApp(); };
    };
}