#pragma once
//=========================================================
// DeviceResource : DirectX Devices
//=========================================================
#include "DXHelper.h"

namespace DX
{
    class DeviceResources 
    {
    public:
        DeviceResources();

        void SetWindow(SDL_Window* window);
        void Present();

        // D3D Accessors
        ID3D11Device*           GetD3Device() const         { return m_device.Get(); }
        ID3D11DeviceContext*    GetD3Context() const        { return m_context.Get(); }
        ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }
        ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }
        D3D11_VIEWPORT          GetScreenViewport() const   { return m_viewPort; }

    private:
        void CreateDeviceResources();      
        void CreateWindowDependentResources();  

        // Cache reference of SDL Window
        SDL_Window*                                 m_window;

        // Direct 3D objects
        Microsoft::WRL::ComPtr<ID3D11Device>        m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<IDXGISwapChain>      m_swapChain;

        // Direct 3D rendering objects
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
        D3D11_VIEWPORT                                 m_viewPort;

        // DirectX device properties
        D3D_FEATURE_LEVEL                           m_featureLevel;

    };

}
