cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
};

cbuffer cbPerFrame : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
};

struct PointVertexOut
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

struct PointVertexIn
{
    float3 pos   : POSITION;
    float3 color : COLOR;
};

PointVertexOut main( PointVertexIn input )
{
    PointVertexOut vout;

    // If no geometry shader then hardware transformation
    // must be done here, vertices must be in proper place
    // after they pass through vertex shader
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, gWorld);
    pos = mul(pos, gView);
    pos = mul(pos, gProj);

    vout.pos = pos;
    vout.color = input.color;

	return vout;
}