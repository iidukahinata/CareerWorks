#include "PBR.hlsli"

Texture2D<float4> g_albedoTex	: register(t0);
Texture2D<float4> g_specularTex : register(t1);
Texture2D<float4> g_normalTex	: register(t2);
Texture2D<float4> g_depthTex	: register(t3);

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

	output.pos = mul(input.pos, worldViewProjection);
	output.tex = input.tex;

	return output;
}

float4 ps_main(PS_IN input) : SV_TARGET
{
	float4 finalColor = float4(0.f, 0.f, 0.f, 1.f);
	
	float4 albedo	= g_albedoTex.Sample(g_sampler, input.tex);
	float4 specular = g_specularTex.Sample(g_sampler, input.tex);
	float4 normal	= g_normalTex.Sample(g_sampler, input.tex);
	float3 depth	= g_depthTex.Sample(g_sampler, input.tex);
	
	// Create Material
	Material material;
	material.albedoColor = albedo.xyz;
	material.metallic	 = specular.x;
	material.roughness	 = specular.y;
	
	float4 worldPos = float4(specular.x, specular.y, depth.z, 0.f);
	float3 toEye = normalize(eyePos - worldPos).xyz;
	
	const uint pointLightCount = lightCount.x;
	const uint spotLightCount  = lightCount.y;

	// Ambient Light
	finalColor.xyz += (ambientColor.xyz * albedo.xyz);
	
	// Directional Light
	finalColor.xyz += PBR(material, ToLightFromDirectionalLight(directionalLight), toEye, normal.xyz);
	
	// Point Light
	for (uint i = 0; i < pointLightCount; ++i)
	{		
		finalColor.xyz += PBR(material, ToLightFromPointLight(pointLights[i], worldPos), toEye, normal.xyz);
	}

	// Spot Light
	for (uint i = 0; i < spotLightCount; ++i)
	{	
		finalColor.xyz += PBR(material, ToLightFromSpotLight(spotLights[i], worldPos), toEye, normal.xyz);
	}
	
	return finalColor;
}