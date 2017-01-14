#include "StormStd.h"
#include "GeometryHelper.h"

using namespace DirectX;
using namespace StormMath;

//----------------------------------------------------------
// Create Grid
//
// Generates a grid with (m-1)(n-1) cells, each cell 
// contains two triangles. 
//
// width : width of grid
// depth : max depth of mesh
// m : x resolution
// n : z resolution
// meshData : reference to mesh being built
//
void GeometryGenerator::CreateGrid(float width, float depth, unsigned short m, unsigned short n, MeshData& meshData)
{
    uint32_t vertexCount = m * n;
    uint32_t faceCount = (m - 1)*(n - 1) * 2;

    // 
    // Create the vertices.
    //
    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;

    float dx = width / (m - 1);
    float dz = width / (n - 1);

    float du = 1.0f / (m - 1);
    float dv = 1.0f / (n - 1);

    meshData.vertices.resize(vertexCount);
    for (uint32_t i = 0; i < n; ++i)
    {
        float z = halfDepth - i*dz;
        for (uint32_t j = 0; j < m; ++j)
        {
            float x = -halfWidth + j*dx;

            meshData.vertices[i*m + j].position = XMFLOAT3(x, 0.0f, z); // no height has been applied

            // TODO: Used for lighting
            meshData.vertices[i*m+ j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f); 
            meshData.vertices[i*m + j].tangentU = XMFLOAT3(1.0f, 0.0f, 0.0f); 

            // TODO: used for texture
            meshData.vertices[i*m + j].texC.x = j*du;
            meshData.vertices[i*m + j].texC.y = j*dv;
        }
    }

    //
    // Create the indices
    //
    meshData.indices.resize(3 * faceCount);

    uint32_t k = 0;
    for (uint32_t i = 0; i < m - 1u; ++i)
    {
        for (uint32_t j = 0; j < n - 1u; ++j)
        {
            meshData.indices[k] = i * m + j;
            meshData.indices[k+1] = i * m + j + 1;
            meshData.indices[k+2] = (i+1) * m + j;
            meshData.indices[k+3] = (i+1) * m + j;
            meshData.indices[k+4] = i * m + j+1;
            meshData.indices[k+5] = (i+1) * m + j + 1;

            k += 6; // next quad
        }
    }

}

//----------------------------------------------------------
// Create Cylinder
//
// Generates a cylinder with (stackCount + 1) rings, and 
// (sliceCount) vertices per ring.
//
// bottomRadius : radius of bottom piece
// topRadius : radius of top piece
// height : height of cylinder
// sliceCount : unique vertices per ring
// stackCount : verticle rings
// meshData : reference to mesh ebing built
//
void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, uint32_t sliceCount, uint32_t stackCount, MeshData& meshData)
{
    meshData.vertices.clear();
    meshData.indices.clear();

    //
    //  Build stack
    // 
    float stackHeight = height / stackCount;
    float radiusStep = (topRadius - bottomRadius) / stackCount;
    uint32_t ringCount = stackCount + 1;
    

    // Comput vertices of stack ring starting at the bottom.
    for (uint32_t i = 0; i < ringCount; i++)
    {
        float y = (-0.5f)*height + i*stackHeight;
        float r = bottomRadius + i * radiusStep;

        float dTheta = 2.0f*XM_PI/sliceCount;
        for (uint32_t j = 0; j < sliceCount; j++)
        {
            MeshVertex vertex;

            float c = cosf(j*dTheta);
            float s = sinf(j*dTheta);

            vertex.position = XMFLOAT3(r*c, y, r*s);
            vertex.texC.x = (float) j/sliceCount;
            vertex.texC.y = 1.0f - (float) i/stackCount;

            vertex.tangentU = XMFLOAT3(-s, 0.0f, c);

            float dr = bottomRadius - topRadius;
            XMFLOAT3 bitangent(dr*c, -height, dr*s);

            XMVECTOR T = XMLoadFloat3(&vertex.tangentU);
            XMVECTOR B = XMLoadFloat3(&bitangent);
            XMVECTOR N = XMVector3Normalize(XMVector3Cross(T,B));
            XMStoreFloat3(&vertex.normal, N);

            meshData.vertices.push_back(vertex);
        }
    }

    // Need to consider duplicate of first and last vertex per ring.
    uint32_t ringVertexCount = sliceCount + 1;
    for (uint32_t i = 0; i < sliceCount; i++)
    {
        for (uint32_t j = 0; j < sliceCount; j++)
        {
            meshData.indices.push_back(i*ringVertexCount + j);
            meshData.indices.push_back((i+1)*ringVertexCount + j);
            meshData.indices.push_back((i+1)*ringVertexCount + (j+1));

            meshData.indices.push_back(i*ringVertexCount + j);
            meshData.indices.push_back((i + 1)*ringVertexCount + (j + 1));
            meshData.indices.push_back(i*ringVertexCount + (j+1));
        }
    }
}

//----------------------------------------------------------
// Create Cylinder
//
// Generates a sphere with 
//
// radius : sphere radius
// numsubDivisions : quality
// meshData : reference to mesh being built
//
void GeometryGenerator::CreateGeosphere(float radius, uint32_t numSubdivisions, MeshData& meshData)
{
    // cap numSubdivisions
    numSubdivisions = min(numSubdivisions, 5u);

    const float X = 0.525731f;
    const float Z = 0.850651f;

    XMFLOAT3 pos[12] = 
    {
        XMFLOAT3(-X, 0.0f, Z), XMFLOAT3(X, 0.0f, Z),
        XMFLOAT3(-X, 0.0f,-Z), XMFLOAT3(X, 0.0f,-Z),
        XMFLOAT3(0.0f, Z, X), XMFLOAT3(0.0f, Z, -X),
        XMFLOAT3(0.0f,-Z, X), XMFLOAT3(0.0f,-Z, -X),
        XMFLOAT3(Z, X, 0.0f), XMFLOAT3(-Z, X, 0.0f),
        XMFLOAT3(Z,-X, 0.0f), XMFLOAT3(-Z,-X, 0.0f)
    };
    
    unsigned short k[60] =
    {
        1,4,0,4,9,0,4,5,9,8,5,4,1,8,4,
        1,10,8,10,3,8,8,3,5,3,2,5,3,7,2,
        3,10,7,10,6,7,6,11,7,6,0,11,6,1,0,
        10,1,6,11,0,9,2,11,9,5,2,9,11,2,7
    };

    meshData.vertices.resize(12);
    meshData.indices.resize(60);

    for(size_t i = 0; i < 12; i++)
        meshData.vertices[i].position = pos[i];
    for(size_t i = 0; i < 60; i++)
        meshData.indices[i] = k[i];

    for (size_t i = 0; i < numSubdivisions; i++)
        Subdivide(meshData);

    // Project vertices onto sphere and scale.
    for (UINT i = 0; i < meshData.vertices.size(); ++i)
    {
        // Project onto unit sphere.
        XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&meshData.vertices[i].position));

        // Project onto sphere.
        XMVECTOR p = radius*n;

        XMStoreFloat3(&meshData.vertices[i].position, p);
        XMStoreFloat3(&meshData.vertices[i].normal, n);

        // Derive texture coordinates from spherical coordinates.
        float theta = AngleFromXY(
            meshData.vertices[i].position.x,
            meshData.vertices[i].position.z);

        float phi = acosf(meshData.vertices[i].position.y / radius);

        meshData.vertices[i].texC.x = theta / XM_2PI;
        meshData.vertices[i].texC.y = phi / XM_PI;

        // Partial derivative of P with respect to theta
        meshData.vertices[i].tangentU.x = -radius*sinf(phi)*sinf(theta);
        meshData.vertices[i].tangentU.y = 0.0f;
        meshData.vertices[i].tangentU.z = +radius*sinf(phi)*cosf(theta);

        XMVECTOR T = XMLoadFloat3(&meshData.vertices[i].tangentU);
        XMStoreFloat3(&meshData.vertices[i].tangentU, XMVector3Normalize(T));
    }
}

void GeometryGenerator::Subdivide(MeshData& meshData)
{
    // Save a copy of the input geometry.
    MeshData inputCopy = meshData;


    meshData.vertices.resize(0);
    meshData.indices.resize(0);

    //       v1
    //       *
    //      / \
    //     /   \
	//  m0*-----*m1
    //  / \     / \
	//  /   \ /   \
	// *-----*-----*
    // v0    m2     v2

    UINT numTris = inputCopy.indices.size() / 3;
    for (UINT i = 0; i < numTris; ++i)
    {
        MeshVertex v0 = inputCopy.vertices[inputCopy.indices[i * 3 + 0]];
        MeshVertex v1 = inputCopy.vertices[inputCopy.indices[i * 3 + 1]];
        MeshVertex v2 = inputCopy.vertices[inputCopy.indices[i * 3 + 2]];

        //
        // Generate the midpoints.
        //

        MeshVertex m0, m1, m2;

        m0.position = Lerp(v0.position, v1.position, 0.5f);
        m1.position = Lerp(v1.position, v2.position, 0.5f);
        m2.position = Lerp(v0.position, v2.position, 0.5f);

        m0.texC = Lerp(v0.texC, v1.texC, 0.5f);
        m1.texC = Lerp(v1.texC, v2.texC, 0.5f);
        m2.texC = Lerp(v0.texC, v2.texC, 0.5f);

        XMStoreFloat3(&m0.normal, XMVector3Normalize(XMLoadFloat3(&Lerp(v0.normal, v1.normal, 0.5f))));
        XMStoreFloat3(&m1.normal, XMVector3Normalize(XMLoadFloat3(&Lerp(v1.normal, v2.normal, 0.5f))));
        XMStoreFloat3(&m2.normal, XMVector3Normalize(XMLoadFloat3(&Lerp(v0.normal, v2.normal, 0.5f))));

        XMStoreFloat3(&m0.tangentU, XMVector3Normalize(XMLoadFloat3(&Lerp(v0.tangentU, v1.tangentU, 0.5f))));
        XMStoreFloat3(&m1.tangentU, XMVector3Normalize(XMLoadFloat3(&Lerp(v1.tangentU, v2.tangentU, 0.5f))));
        XMStoreFloat3(&m2.tangentU, XMVector3Normalize(XMLoadFloat3(&Lerp(v0.tangentU, v2.tangentU, 0.5f))));

        //
        // Add new geometry.
        //

        meshData.vertices.push_back(v0); // 0
        meshData.vertices.push_back(v1); // 1
        meshData.vertices.push_back(v2); // 2
        meshData.vertices.push_back(m0); // 3
        meshData.vertices.push_back(m1); // 4
        meshData.vertices.push_back(m2); // 5

        meshData.indices.push_back(i * 6 + 0);
        meshData.indices.push_back(i * 6 + 3);
        meshData.indices.push_back(i * 6 + 5);
                 
        meshData.indices.push_back(i * 6 + 3);
        meshData.indices.push_back(i * 6 + 4);
        meshData.indices.push_back(i * 6 + 5);
                 
        meshData.indices.push_back(i * 6 + 5);
        meshData.indices.push_back(i * 6 + 4);
        meshData.indices.push_back(i * 6 + 2);
                 
        meshData.indices.push_back(i * 6 + 3);
        meshData.indices.push_back(i * 6 + 1);
        meshData.indices.push_back(i * 6 + 4);
    }
}

float GeometryGenerator::AngleFromXY(float x, float y)
{
    float theta = 0.0f;

    // Quadrant I or IV
    if (x >= 0.0f)
    {
        // If x = 0, then atanf(y/x) = +pi/2 if y > 0
        //                atanf(y/x) = -pi/2 if y < 0
        theta = atanf(y / x); // in [-pi/2, +pi/2]

        if (theta < 0.0f)
            theta += 2.0f*M_PI; // in [0, 2*pi).
    }

    // Quadrant II or III
    else
        theta = atanf(y / x) + M_PI; // in [0, 2*pi).

    return theta;
}

DirectX::XMFLOAT2 GeometryGenerator::Lerp(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b, float t)
{
    return DirectX::XMFLOAT2(
        (1.0f - t)*a.x + t*b.x,
        (1.0f - t)*a.y + t*b.y);
}

DirectX::XMFLOAT3 GeometryGenerator::Lerp(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float t)
{
    return DirectX::XMFLOAT3(
        (1.0f - t)*a.x + t*b.x,
        (1.0f - t)*a.y + t*b.y,
        (1.0f - t)*a.z + t*b.z);
}

DirectX::XMFLOAT4 GeometryGenerator::Lerp(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b, float t)
{
    return DirectX::XMFLOAT4(
        (1.0f - t)*a.x + t*b.x,
        (1.0f - t)*a.y + t*b.y,
        (1.0f - t)*a.z + t*b.z,
        (1.0f - t)*a.w + t*b.w);
}
