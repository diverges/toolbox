#pragma once

namespace StormMath
{

    static DirectX::XMFLOAT4X4 Identity4x4()
    {
        static DirectX::XMFLOAT4X4 I(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        return I;
    }

    DirectX::XMFLOAT3 ComputeNormal(
        const DirectX::XMFLOAT3& v1,
        const DirectX::XMFLOAT3& v2, 
        const DirectX::XMFLOAT3& v3);

    DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX& M);
}
