#include "Core.hlsli"

struct VS_IN
{
	float4 pos : POSITION;
};

float4 vs_main(VS_IN input) : SV_Position
{
	return mul(worldViewProjection, input.pos);
}