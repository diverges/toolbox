#pragma once
#include <vector>
#include <DirectXMath.h>

namespace StormMath
{
namespace GeometryGenerator
{
    struct MeshVertex
    {
        MeshVertex() {};
        MeshVertex(
            const DirectX::XMFLOAT3& p,
            const DirectX::XMFLOAT3& n,
            const DirectX::XMFLOAT3& t,
            const DirectX::XMFLOAT3& uv)
            : position(p), normal(n), tangentU(t), texC(uv) {}
        MeshVertex(
            float px, float py, float pz,
            float nx, float ny, float nz,
            float tx, float ty, float tz,
            float cx, float cy, float cz)
            : position(px,py,pz), normal(nx, ny, nz), 
              tangentU(tx, ty, tz), texC(cx, cy, cz) {}
                

        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT3 tangentU;
        DirectX::XMFLOAT3 texC;
    };

    struct MeshData
    {
        std::vector<MeshVertex> vertices;
        std::vector<unsigned short> indices;

        MeshData operator+(const MeshData& other)
        {
            MeshData temp;
            temp.vertices.reserve(other.vertices.size() + vertices.size());
            temp.indices.reserve(other.indices.size() + indices.size());

            temp.vertices.insert(temp.vertices.end(), other.vertices.begin(), other.vertices.end());
            temp.vertices.insert(temp.vertices.end(), vertices.begin(), vertices.end());
            temp.indices.insert(temp.indices.end(), other.indices.begin(), other.indices.end());
            temp.indices.insert(temp.indices.end(), indices.begin(), indices.end());
            return temp;
        }
    };

    

    // 
    // Shape Generator
    //
    void CreateGrid(float width, float depth, unsigned short m, unsigned short n, MeshData& meshData);
    void CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData);
    void CreateGeosphere(float radius, uint32_t numSubdivisions, MeshData& data);

    // Helper
    void  Subdivide(MeshData& meshData);
    float AngleFromXY(float x, float y);

    DirectX::XMFLOAT2 Lerp(const DirectX::XMFLOAT2 & a, const DirectX::XMFLOAT2 & b, float t);

    DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b, float t);

    DirectX::XMFLOAT4 Lerp(const DirectX::XMFLOAT4 & a, const DirectX::XMFLOAT4 & b, float t);
}
}
