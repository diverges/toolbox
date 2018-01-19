#pragma once

namespace DX
{
    struct Material
    {
        Material() { ZeroMemory(this, sizeof(this)); }

        DirectX::XMFLOAT4 Ambient;
        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Specular;
        DirectX::XMFLOAT4 Reflect;
    };

    struct DirectionalLight
    {
        DirectionalLight() { ZeroMemory(this, sizeof(this)); }

        DirectX::XMFLOAT4 Ambient;
        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Specular;

        DirectX::XMFLOAT3 Direction;
        float pad;

    };

    struct PointLight
    {
        PointLight() { ZeroMemory(this, sizeof(this)); }

        DirectX::XMFLOAT4 Ambient;
        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Specular;

        DirectX::XMFLOAT3 Position;
        float range;

        DirectX::XMFLOAT3 Att;
        float pad;
    };

    struct SpotLight
    {
        SpotLight() { ZeroMemory(this, sizeof(this)); }

        DirectX::XMFLOAT4 Ambient;
        DirectX::XMFLOAT4 Diffuse;
        DirectX::XMFLOAT4 Specular;

        DirectX::XMFLOAT3 Position;
        float range;

        DirectX::XMFLOAT3 Direction;
        float spot;

        DirectX::XMFLOAT3 Att;
        float pad;
    };
}