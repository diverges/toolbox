#include "StormStd.h"
#include "D3DRender.h"

using namespace DirectX;

DX::D3DRender::D3DRender(const std::shared_ptr<DX::DeviceResources>& device) :
    m_deviceResources(device)
{
    CreateDeviceResources();
    CreateWindowDependentResources();
}

void DX::D3DRender::CreateDeviceResources()
{

    CONSOLE_OUT("\n\nStarting D3DRender creation---\n");

    //
    // Input Layouts
    //
    D3D11_INPUT_ELEMENT_DESC pointVertex [] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    DX::ThrowIfFailed(
        // PointVertex Layout
        m_deviceResources->GetD3Device()->CreateInputLayout(
            pointVertex,
            ARRAYSIZE(pointVertex),
            &g_vs_point,
            ARRAYSIZE(g_vs_point),
            &m_inputLayout
        )
    );
    CONSOLE_OUT("\tInput layout built...\n");

    //
    // Constant Buffer : perObject
    //
    CD3D11_BUFFER_DESC constantBufferDesc1(sizeof(cbPerObject), D3D11_BIND_CONSTANT_BUFFER);
    DX::ThrowIfFailed(
        m_deviceResources->GetD3Device()->CreateBuffer(
            &constantBufferDesc1,
            nullptr,
            &m_objectCB
        )
    );
    CD3D11_BUFFER_DESC constantBufferDesc2(sizeof(cbPerFrame), D3D11_BIND_CONSTANT_BUFFER);
    DX::ThrowIfFailed(
        m_deviceResources->GetD3Device()->CreateBuffer(
            &constantBufferDesc2,
            nullptr,
            &m_frameCB
        )
    );
    CONSOLE_OUT("\tConstant buffer built...\n");

    
    // HACK :
    // TODO : this is a test 
    //
    // Generate Grid
    StormMath::GeometryGenerator::MeshData grid;
    StormMath::GeometryGenerator::CreateGrid(1000.0f, 160.0f, 160, 160, grid);
    uint32_t gridSize = grid.vertices.size();
    uint32_t indexSize = grid.indices.size();

    //GeometryGenerator::CreateCylinder(50.0f, 50.0f, 100.0f, 50, 50, grid);
    //StormMath::GeometryGenerator::CreateGeosphere(70.0f, 15, grid);

    StormMath::GeometryGenerator::MeshData grid2;
    StormMath::GeometryGenerator::CreateGrid(1000.0f, 160.0f, 160, 160, grid2);
    grid = grid + grid2;

    m_indexCount = grid.indices.size();
    std::vector<DX::PointVertex> vertices(grid.vertices.size());
    uint32_t i = 0;
    for (; i < gridSize; i++)
    {
        XMFLOAT3 p = grid.vertices[i].position;
        p.y = 0.3f*(p.z*sinf(0.1f*p.x) + p.x * cosf(0.1f*p.z));
        vertices[i].Pos = p;
        DirectX::XMStoreFloat3(&vertices[i].Color, DirectX::Colors::DarkGray);
    }
    for (; i < grid.vertices.size(); i++)
    {
        XMFLOAT3 p = grid.vertices[i].position;
        p.y = -5.0f;
        vertices[i].Pos = p;
        
        DirectX::XMStoreFloat3(&vertices[i].Color, DirectX::Colors::Blue);
    }
    for (uint32_t j = indexSize; j < m_indexCount; j++)
    {
        grid.indices[j] += gridSize;
    }

    const UINT vbdByteSize = sizeof(PointVertex) * vertices.size();
    const UINT idbByteSize = sizeof(unsigned short) * m_indexCount;

    m_demoMesh = std::make_unique<MeshGeometry>();
    m_demoMesh->Name = "hills";

    DX::ThrowIfFailed(D3DCreateBlob(vbdByteSize, &m_demoMesh->VertexBufferCPU));
    CopyMemory(m_demoMesh->VertexBufferCPU->GetBufferPointer(), &grid.vertices[0], vbdByteSize);

    DX::ThrowIfFailed(D3DCreateBlob(idbByteSize, &m_demoMesh->IndexBufferCPU));
    CopyMemory(m_demoMesh->IndexBufferCPU->GetBufferPointer(), &grid.vertices[0], idbByteSize);

    m_demoMesh->VertexBufferGPU = DX::CreateDefaultBuffer(m_deviceResources->GetD3Device(), vertices.data(), vbdByteSize, D3D11_BIND_VERTEX_BUFFER);
    CONSOLE_OUT("\tVertex buffer built...\n");

    m_demoMesh->IndexBufferGPU = DX::CreateDefaultBuffer(m_deviceResources->GetD3Device(), grid.indices.data(), idbByteSize, D3D11_BIND_INDEX_BUFFER);
    CONSOLE_OUT("\tIndex buffer built...\n");

    m_demoMesh->VertexBufferStride = sizeof(PointVertex);
    m_demoMesh->VertexBufferByteSize = vbdByteSize;
    m_demoMesh->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_demoMesh->IndexBufferByteSize = idbByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = m_indexCount;
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    m_demoMesh->DrawArgs["box"] = submesh;

    // Add items to render
    auto demoRender = std::make_unique<RenderItem>();
    demoRender->Geometry = m_demoMesh.get();
    demoRender->IndexCount = m_demoMesh->DrawArgs["box"].IndexCount;
    demoRender->StartIndexLocation = m_demoMesh->DrawArgs["box"].StartIndexLocation;
    demoRender->BaseVertexLocation = m_demoMesh->DrawArgs["box"].BaseVertexLocation;

    m_visibleLayer.push_back(demoRender.get());
    m_allRenderItems.push_back(std::move(demoRender));


    //
    // Create shaders
    //
    DX::ThrowIfFailed(
        m_deviceResources->GetD3Device()->CreateVertexShader(&g_vs_point, ARRAYSIZE(g_vs_point), nullptr, &m_vertexShader)
    );
    DX::ThrowIfFailed(
        m_deviceResources->GetD3Device()->CreatePixelShader(&g_ps_simple, ARRAYSIZE(g_ps_simple), nullptr, &m_pixelShader)
    );
    CONSOLE_OUT("\tVertex and pixel shader built...\n");
    
}

void DX::D3DRender::CreateWindowDependentResources()
{

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    XMMATRIX I = XMMatrixIdentity();
    XMStoreFloat4x4(&m_perObjectData.gWorld, I); 

    static const XMVECTOR eye = { 175.0f, 100.0f, 25.0f, 1.0f };
    static const XMVECTOR at = { 0.0f, 0.0f, -10.0f, 0.0f };
    static const XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
    XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
    XMStoreFloat4x4(&m_perFrameData.gView, XMMatrixTranspose(view));

    float fovAngleY = 70.0f * XM_PI / 180.0f;
    XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(
        fovAngleY = 0.25f*3.11159f,
        (float)(SCREEN_WIDTH / SCREEN_HEIGHT),
        1.0f,
        1000.0f
    );
    XMStoreFloat4x4(&m_perFrameData.gProj, XMMatrixTranspose(perspectiveMatrix));
}

void DX::D3DRender::Clear()
{
    //
    // 
    //

    auto context = m_deviceResources->GetD3Context();

    // Reset the viewport to target the whole screen.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    //
    // Set render target views to the screen
    ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetRenderTargetView() };
    context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

    // Clear the back buffer and depth stencil view.
    context->ClearRenderTargetView(m_deviceResources->GetRenderTargetView(), 
        DirectX::Colors::CornflowerBlue);
    context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), 
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX::D3DRender::Render()
{
    //
    // Clean viewport and views
    Clear();
    
    //
    // Grab device contect from DeviceResource.
    auto context = m_deviceResources->GetD3Context();

    //
    // BEGIN: Buffer initialization and update
    context->UpdateSubresource(m_frameCB.Get(), 0, NULL, &m_perFrameData, 0, 0);

    // Send the constant buffer to the graphics device.
    context->VSSetConstantBuffers(0, 1, m_objectCB.GetAddressOf());
    context->VSSetConstantBuffers(1, 1, m_frameCB.GetAddressOf());

    //
    // Bind vertex buffer to input
    UINT stride = m_demoMesh->VertexBufferStride;
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_demoMesh->VertexBufferGPU.GetAddressOf()
, &stride, &offset);
    //
    // Bind index buffer 
    // Each index is one 16-bit unsigned integer (short).
    context->IASetIndexBuffer(m_demoMesh->IndexBufferGPU.Get(), m_demoMesh->IndexFormat, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //
    // Set input topology
    context->IASetInputLayout(m_inputLayout.Get());
    //
    // END: Buffer initialization and update

    //
    // BEGIN: Attach shaders
    //
    // Attach our vertex shader.
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    // Attach our pixel shader.
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    //
    // END: Attach shaders
    //

    //
    // Draw from index buffer
    //
    DrawRenderItems(context, m_visibleLayer);

}

void DX::D3DRender::DrawRenderItems(ID3D11DeviceContext* context, const std::vector<RenderItem*>& items)
{
    for (size_t i = 0; i < items.size(); i++)
    {
        auto cur = items[i];

        // Set Input and Vertex Buffer
        UINT stride = m_demoMesh->VertexBufferStride;
        UINT offset = 0;
        context->IASetVertexBuffers(0,1, cur->Geometry->VertexBufferGPU.GetAddressOf(), &stride, &offset);
        context->IASetPrimitiveTopology(cur->PrimitiveType);

        //
        // Update constant buffer
        cbPerObject objectData;
        XMMATRIX world = XMLoadFloat4x4(&cur->World);
        XMStoreFloat4x4(&objectData.gWorld, XMMatrixTranspose(world));
        context->UpdateSubresource(m_objectCB.Get(), 0, NULL, &objectData, 0, 0);

        // Draw
        //context->DrawIndexedInstanced(cur->IndexCount, 1, cur->StartIndexLocation, cur->BaseVertexLocation, 0);
        context->DrawIndexed(cur->IndexCount, 0, 0);
    } 
}
