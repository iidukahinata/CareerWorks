#include "PostProcess.hlsli"

#if UseLumaCRV
cbuffer ConstBufferLuma : register(b1)
{
	float intensity;
	float threshold;
	float _clamp;
};
#endif

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

PS_IN VS_LuminousPass(VS_IN input)
{
	PS_IN output;

	output.pos = mul(worldViewProjection, input.pos);
	output.tex = input.tex;

	return output;
}

float4 PS_LuminousPass(PS_IN input)
{
	float4 finalColor = g_lightingTex.Sample(g_sampler, input.tex);

#if UseLumaCRV
	float luma = dot(finalColor.xyz, float3(0.2125f, 0.7154f, 0.0721f));	
	float val  = clamp(luma - threshold, 0.0f, _clamp) * intensity;
#else
	float luma = dot(finalColor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	float val = clamp(luma - 1.f, 0.0f, 1.0f);
#endif
	
	return finalColor * val;
}