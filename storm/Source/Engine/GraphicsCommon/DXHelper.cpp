#include "StormStd.h"
#include "DXHelper.h"

#include "Utilities\MathHelper.h"

Microsoft::WRL::ComPtr<ID3D11Buffer> DX::CreateDefaultBuffer(ID3D11Device * device, const void * initData, UINT byteSize, D3D11_BIND_FLAG  bindFlags)
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> defaultBuffer;

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = byteSize;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = bindFlags;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vData;
    vData.pSysMem = initData;
    vData.SysMemPitch = 0;
    vData.SysMemSlicePitch = 0;
    DX::ThrowIfFailed(
        device->CreateBuffer(&desc, &vData, defaultBuffer.GetAddressOf())
    );

    return defaultBuffer;
}

void DX::ThrowIfFailed(HRESULT hr)
{
#if defined(DEBUG) || defined(_DEBUG)
    if (FAILED(hr))
    {
        g_pApp->DebugLog("DX error hr: %x\n", (int)hr);
    }
#endif    
}

int DX::CalculateFrameStats(const DX::GameTimer& timer)
{
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;

    if ((timer.GetTotalTime() - timeElapsed) >= 1.0f)
    {
        float fps = (float)frameCnt;
        float mspf = 1000 / fps;

        g_pApp->DebugLog("[ Total Time: %f - FPS: %f - Fametime: %f ]\n", timer.GetTotalTime(), fps, mspf);

        frameCnt = 0;
        timeElapsed += 1.0f;
    }

    return 0;
}
