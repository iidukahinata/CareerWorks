#include "Core.hlsli"

Texture2D<float4> g_albedoTex	: register(t0);
Texture2D<float4> g_specularTex : register(t1);
Texture2D<float4> g_normalTex	: register(t2);
Texture2D<float4> g_depthTex	: register(t3);
Texture2D<float4> g_PositionTex : register(t4);
Texture2D<float4> g_lightingTex : register(t5);
sampler g_sampler;

struct VS_IN
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct PS_IN 
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

PS_IN vs_main(VS_IN input)
{
	PS_IN output;

	output.pos = mul(worldViewProjection, input.pos);
	output.tex = input.tex;

	return output;
}

#if Luma
float4 ps_main(PS_IN input) : SV_TARGET
{
	float4 finalColor = g_lightingTex.Sample(g_sampler, input.tex);

	float t = dot(finalColor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	
	clip(t - 1.f);
	return finalColor;
}
#endif