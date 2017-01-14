float4 main(
    float3 pos  : POSITION, 
    float3 norm : NORMAL,
    float2 tex0 : TEXCOORD0,
    float2 tex1 : TEXCOORD1
) : SV_POSITION
{
	return float4(pos, 1.0);
}