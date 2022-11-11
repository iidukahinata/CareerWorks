#include "Core.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer ConstBufferMatrix : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VS_IN 
{
    float4 pos	: POSITION;
	float2 tex	: TEXCOORD0;
};

struct PS_IN
{
	float4 pos	: SV_POSITION;
	float3 tex	: TEXCOORD0;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;

	output.pos = mul(worldViewProjection, input.pos);	
	output.tex = input.tex;

	return output;
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	return g_cubeMap.Sample(g_sampler, input.tex);
}