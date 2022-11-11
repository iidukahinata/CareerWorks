#include "PostProcess.hlsli"

Texture2D<float4> g_texture0 : register(t7);
Texture2D<float4> g_texture1 : register(t8);
Texture2D<float4> g_texture2 : register(t9);
Texture2D<float4> g_texture3 : register(t10);

cbuffer ConstantBufferBloom : register(b1)
{
	float4 color;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

PS_IN VS_BloomPass(VS_IN input)
{
	PS_IN output;

	output.pos = mul(worldViewProjection, input.pos);
	output.tex = input.tex;

	return output;
}

float4 PS_BloomPass(PS_IN input)
{
	float4 finalColor;	
	finalColor = g_lightingTex.Sample(g_sampler, input.tex);
	
	float4 addColor;
	addColor  = g_texture0.Sample(g_sampler, input.tex);
	addColor += g_texture1.Sample(g_sampler, input.tex);
	addColor += g_texture2.Sample(g_sampler, input.tex);
	addColor += g_texture3.Sample(g_sampler, input.tex);
	addColor /= 4.f;
	
	// éwíËêFÇ≈â¡éZçáê¨
	finalColor += (addColor * color);

	return float4(finalColor.xyz, 1.0f);
}