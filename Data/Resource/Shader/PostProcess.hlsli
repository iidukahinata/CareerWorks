#include "Core.hlsli"

Texture2D<float4> g_albedoTex	: register(t0);
Texture2D<float4> g_specularTex : register(t1);
Texture2D<float4> g_normalTex	: register(t2);
Texture2D<float4> g_depthTex	: register(t3);
Texture2D<float4> g_PositionTex : register(t4);
Texture2D<float4> g_lightingTex : register(t5);
Texture2D<float4> g_luminousTex : register(t6);
sampler g_sampler;

struct VS_IN
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};