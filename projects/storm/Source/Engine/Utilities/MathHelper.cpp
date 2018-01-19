#include "StormStd.h"
#include "MathHelper.h"

using namespace DirectX;

DirectX::XMFLOAT3 StormMath::ComputeNormal(const DirectX::XMFLOAT3 & p1, const DirectX::XMFLOAT3 & p2, const DirectX::XMFLOAT3 & p3)
{
    XMFLOAT3 ret;
    XMVECTOR v1 = XMLoadFloat3(&p1);
    XMVECTOR v2 = XMLoadFloat3(&p2);
    XMVECTOR v3 = XMLoadFloat3(&p3);
    XMStoreFloat3(&ret, XMVector3Cross(XMVectorSubtract(v2, v1), XMVectorSubtract(v3, v1)));
    return ret;
}

DirectX::XMMATRIX StormMath::InverseTranspose(DirectX::CXMMATRIX & M)
{
    XMMATRIX a = M;
    a.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR det = XMMatrixDeterminant(a);
    return XMMatrixTranspose(XMMatrixInverse(&det, a));
}
