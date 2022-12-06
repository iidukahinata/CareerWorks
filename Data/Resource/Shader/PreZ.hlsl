#include "Core.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
	
#if USE_INSTANCING
	float4x4 MVP	: MATRIX;
#endif
	
	uint InstanceId : SV_InstanceID;
};

float4 vs_main(VS_IN input) : SV_Position
{
#if USE_INSTANCING
	return mul(input.MVP, input.pos);
#else
	return mul(worldViewProjection, input.pos);
#endif	
}