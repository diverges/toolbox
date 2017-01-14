#pragma once

namespace DX
{
    struct cbPerObject
    {
        DirectX::XMFLOAT4X4 gWorld;
    };

    struct cbPerFrame
    {
        DirectX::XMFLOAT4X4 gView;
        DirectX::XMFLOAT4X4 gProj;
    };

    struct PointVertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT3 Color;
    };

    struct TexVertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT3 Normal;
        DirectX::XMFLOAT2 Tex0;
        DirectX::XMFLOAT2 Tex1;
    };
}