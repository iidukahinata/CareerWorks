#include "PostProcess.hlsli"

Texture2D<float4> g_texture : register(t7);

cbuffer ConstBufferBlur : register(b1)
{
	float4 weights[2];
	float2 screenSize;
};

struct PS_IN
{
	float4 pos  : SV_POSITION;
	float4 tex1 : TEXCOORD1;
	float4 tex2 : TEXCOORD2;
	float4 tex3 : TEXCOORD3;
	float4 tex4 : TEXCOORD4;
	float4 tex5 : TEXCOORD5;
	float4 tex6 : TEXCOORD6;
	float4 tex7 : TEXCOORD7;
	float4 tex8 : TEXCOORD8;
};

PS_IN VS_BlurPass(VS_IN input)
{
	PS_IN output;

	output.pos = mul(worldViewProjection, input.pos);
    
#if XBlur
    // テクセルをずらし周辺からサンプリングするためのUV値
    output.tex1.xy = float2( 1.0f / screenSize.x, 0.0f);
    output.tex2.xy = float2( 3.0f / screenSize.x, 0.0f);
    output.tex3.xy = float2( 5.0f / screenSize.x, 0.0f);
    output.tex4.xy = float2( 7.0f / screenSize.x, 0.0f);
    output.tex5.xy = float2( 9.0f / screenSize.x, 0.0f);
    output.tex6.xy = float2(11.0f / screenSize.x, 0.0f);
    output.tex7.xy = float2(13.0f / screenSize.x, 0.0f);
	output.tex8.xy = float2(15.0f / screenSize.x, 0.0f);
#endif

#if YBlur
    // テクセルをずらし周辺からサンプリングするためのUV値
    output.tex1.xy = float2(0.0f,  1.0f / screenSize.y);
    output.tex2.xy = float2(0.0f,  3.0f / screenSize.y);
    output.tex3.xy = float2(0.0f,  5.0f / screenSize.y);
    output.tex4.xy = float2(0.0f,  7.0f / screenSize.y);
    output.tex5.xy = float2(0.0f,  9.0f / screenSize.y);
    output.tex6.xy = float2(0.0f, 11.0f / screenSize.y);
    output.tex7.xy = float2(0.0f, 13.0f / screenSize.y);
    output.tex8.xy = float2(0.0f, 15.0f / screenSize.y);
#endif

    // 反対方向のUV値を求める
    output.tex1.zw = output.tex1.xy * -1.0f;
    output.tex2.zw = output.tex2.xy * -1.0f;
    output.tex3.zw = output.tex3.xy * -1.0f;
    output.tex4.zw = output.tex4.xy * -1.0f;
    output.tex5.zw = output.tex5.xy * -1.0f;
    output.tex6.zw = output.tex6.xy * -1.0f;
    output.tex7.zw = output.tex7.xy * -1.0f;
    output.tex8.zw = output.tex8.xy * -1.0f;

    // 実際のUV座標に変換
    output.tex1 += float4(input.tex, input.tex);
    output.tex2 += float4(input.tex, input.tex);
    output.tex3 += float4(input.tex, input.tex);
    output.tex4 += float4(input.tex, input.tex);
    output.tex5 += float4(input.tex, input.tex);
    output.tex6 += float4(input.tex, input.tex);
    output.tex7 += float4(input.tex, input.tex);
    output.tex8 += float4(input.tex, input.tex);

    return output;
}

float4 PS_BlurPass(PS_IN input)
{
	float4 tex[8] =
	{
		input.tex1, input.tex2, input.tex3, input.tex4,
        input.tex5, input.tex6, input.tex7, input.tex8
	};

	float w[8] =
	{
		weights[0].x, weights[0].y, weights[0].z, weights[0].w,
        weights[1].x, weights[1].y, weights[1].z, weights[1].w
	};

	float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
	for (int i = 0; i < 8; i++)
	{
#if UseLuma
        finalColor += (g_luminousTex.Sample(g_sampler, tex[i].xy) + g_luminousTex.Sample(g_sampler, tex[i].zw)) * w[i];
#else
		finalColor += (g_texture.Sample(g_sampler, tex[i].xy) + g_texture.Sample(g_sampler, tex[i].zw)) * w[i];
#endif
	}

	return float4(finalColor.xyz, 1.0f);
}