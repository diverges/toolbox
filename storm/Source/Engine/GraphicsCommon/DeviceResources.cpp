#include "StormStd.h"
#include "DeviceResources.h"

DX::DeviceResources::DeviceResources() :
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
    CreateDeviceResources();
}

void DX::DeviceResources::CreateDeviceResources()
{
    // Create the DX device and context
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Don't forget to declare your application's minimum required feature level in its
    // description.  All applications are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
   
    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        creationFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &device,
        &m_featureLevel,
        &context
    );
    
    // Store d3 api device and immediate context pointers
    DX::ThrowIfFailed(
        device.As(&m_device)
    );
    DX::ThrowIfFailed(
        context.As(&m_context)
    );
    
}

void DX::DeviceResources::CreateWindowDependentResources()
{
    CONSOLE_OUT("\n\nStarting window dependent device resources creation ---\n");

    // Create Swap Chain
    if (m_swapChain == nullptr)
    {
        DXGI_SWAP_CHAIN_DESC sd = { 0 };
        sd.BufferDesc.Width = SCREEN_WIDTH;
        sd.BufferDesc.Height = SCREEN_HEIGHT;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 2;
        

        // Get HWND from SDL_WIndow
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        DX::ThrowIfFailed(
            SDL_GetWindowWMInfo(m_window, &info)
        );
        sd.OutputWindow = info.info.win.window;
        sd.Flags = 0;
        sd.Windowed = true;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

        CONSOLE_OUT("\tCreating Swap chain...\n");

        // Obtain DXGI factory used to create Direct3D devices
        Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
        DX::ThrowIfFailed(
            m_device.As(&dxgiDevice)
        );

        Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(
            dxgiDevice->GetAdapter(&dxgiAdapter)
        );

        Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
        DX::ThrowIfFailed(
            dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
        );

        // Create the swap chain
        Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
        DX::ThrowIfFailed(
            dxgiFactory->CreateSwapChain(
                m_device.Get(),
                &sd,
                &swapChain
            )
        );
        DX::ThrowIfFailed(
            swapChain.As(&m_swapChain)
        );

        CONSOLE_OUT("\tSwap chain built...\n");

    }

    // Create render target view
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer)
    );
    DX::ThrowIfFailed(
        m_device->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_renderTargetView
        )
    );

    CONSOLE_OUT("\tRender target view built...\n");

    // Create depth stencil view
    // Create a depth stencil view for use with 3D rendering if needed.
    CD3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = SCREEN_WIDTH;
    depthStencilDesc.Height = SCREEN_HEIGHT;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(
        m_device->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            &depthStencil
        )
    );

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(
        m_device->CreateDepthStencilView(
            depthStencil.Get(),
            &depthStencilViewDesc,
            &m_depthStencilView
        )
    );

    CONSOLE_OUT("\tDepth stencil view built...\n");

    // Set viewport
    m_viewPort = CD3D11_VIEWPORT(
        0.0f,
        0.0f,
        (float)SCREEN_WIDTH,
        (float)SCREEN_HEIGHT
    );
    m_context->RSSetViewports(1, &m_viewPort);

    CONSOLE_OUT("\tView port built...\n");

}

void DX::DeviceResources::SetWindow(SDL_Window * window)
{
    m_window = window;

    CreateWindowDependentResources();
}


void DX::DeviceResources::Present()
{
    HRESULT hr = m_swapChain->Present(1, 0);
    /*
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    DXGI_PRESENT_PARAMETERS parameters = { 0 };
    HRESULT hr = m_swapChain->Present(1, 0, &parameters);

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    m_context->DiscardView(m_renderTargetView.Get(), nullptr, 0);

    // Discard the contents of the depth stencil.
    m_context->DiscardView(m_depthStencilView.Get(), nullptr, 0);
    */
}
