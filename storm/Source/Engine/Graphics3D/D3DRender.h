#pragma once
#include "..\GraphicsCommon\DXHelper.h"
#include "..\GraphicsCommon\DeviceResources.h"
#include "ShaderStructures.h"

// Geometry
#include "..\Geometry\GeometryHelper.h"

// Shaders
#include "PointVertexShader.h"
#include "SimplePixelShader.h"

namespace DX
{

    class D3DRender
    {
    public:
        D3DRender(const std::shared_ptr<DX::DeviceResources>& device);
        void CreateDeviceResources();
        void CreateWindowDependentResources();

        void Clear();
        void Render();

    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources>      m_deviceResources;

        // Direct3D resources for 3D geometry
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; // pointVertex layout

        // Constant Buffers
        Microsoft::WRL::ComPtr<ID3D11Buffer>      m_objectCB;
        Microsoft::WRL::ComPtr<ID3D11Buffer>      m_frameCB;

        // Scene Render Items
        std::vector<std::unique_ptr<RenderItem>>  m_allRenderItems;

        // Scene Render Layers 
        std::vector<RenderItem*>                  m_visibleLayer;

        // Direct3D world geometry
        std::unique_ptr<MeshGeometry>             m_demoMesh;

        // Direct3D system resource properties
        uint32_t                                  m_indexCount;
        cbPerObject                               m_perObjectData;
        cbPerFrame                                m_perFrameData;

        // Direct3D shaders
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

        // Constant buffer properties
        DirectX::XMFLOAT4                         m_World;
        DirectX::XMFLOAT4                         m_View;
        DirectX::XMFLOAT4                         m_Projection;

        // Draws all render items
        void DrawRenderItems(ID3D11DeviceContext* context, const std::vector<RenderItem*>& items);

    };
}