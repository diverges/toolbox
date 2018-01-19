#pragma once
#include <unordered_map>

#include "..\Debugger\DebugConsole.h"
#include "..\Utilities\GameTimer.h"
#include "..\Utilities\MathHelper.h"

namespace DX
{
    //
    // Mesh Geometry
    //
    struct SubmeshGeometry
    {
        UINT IndexCount = 0;
        UINT StartIndexLocation = 0;
        INT  BaseVertexLocation = 0;

    };

    struct MeshGeometry
    {
        // Give it a name for later lookup
        std::string Name;

        // CPU Instances
        Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU;
        Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU;

        // GPU Instances
        Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBufferGPU;
        Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBufferGPU;

        // Buffer Data
        UINT VertexBufferStride = 0;
        UINT VertexBufferByteSize = 0;
        DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
        UINT IndexBufferByteSize = 0;

        std::unordered_map<std::string, SubmeshGeometry> DrawArgs;
    };

    // Render Item :
    //  Describes a render object being drawn. Contains topology,
    //  geometry data, and per object constant buffer data.
    struct RenderItem
    {
        RenderItem() = default;

        // Per object constant buffer information
        DirectX::XMFLOAT4X4 World = StormMath::Identity4x4();

        // Render Items can share mesh geometry.
        MeshGeometry* Geometry = nullptr;

        D3D11_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        UINT IndexCount = 0;
        UINT StartIndexLocation = 0;
        int  BaseVertexLocation = 0;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> CreateDefaultBuffer(
        ID3D11Device* device,
        const void* initData,
        UINT stride,
        D3D11_BIND_FLAG bindFlags
    );

    namespace Color
    {
        static const DirectX::XMFLOAT3 White    = { 1.0f, 1.0f, 1.0f };
        static const DirectX::XMFLOAT3 Black    = { 0.0f, 0.0f, 0.0f };
        static const DirectX::XMFLOAT3 Red      = { 1.0f, 0.0f, 0.0f };
        static const DirectX::XMFLOAT3 Green    = { 0.0f, 1.0f, 0.0f };
        static const DirectX::XMFLOAT3 Blue     = { 0.0f, 0.0f, 1.0f };
        static const DirectX::XMFLOAT3 Yellow   = { 1.0f, 1.0f, 0.0f };
        static const DirectX::XMFLOAT3 Cyan     = { 0.0f, 1.0f, 1.0f };
        static const DirectX::XMFLOAT3 Magenta  = { 1.0f, 0.0f, 1.0f };
    }

    void ThrowIfFailed(HRESULT hr);

    int CalculateFrameStats(const DX::GameTimer& timer);
}