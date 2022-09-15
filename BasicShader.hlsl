
Texture2D<float4> tex:register(t0);
Texture2D<float4> sph:register(t1);
Texture2D<float4> spa:register(t2);
Texture2D<float4> toon:register(t3);

SamplerState smp : register(s0);
SamplerState smpToon : register(s1);

cbuffer SceneData : register(b0) 
{
	matrix view;
	matrix proj;
	float3 eye;
};

cbuffer Transform : register(b1) 
{
	matrix world;
}

cbuffer Material : register(b2) 
{
	float4 diffuse;
	float4 specular;
	float3 ambient;
};

struct Input
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TEXCOORD1;
	float3 biNormal : TEXCOORD2;
};

struct Output 
{
	float4 svpos:SV_POSITION;//システム用頂点座標
	float4 pos:POSITION;//システム用頂点座標
	float4 normal:NORMAL0;//法線ベクトル
	float4 vnormal:NORMAL1;//法線ベクトル
	float2 uv:TEXCOORD;//UV値
	float3 ray:VECTOR;//ベクトル
};

Output vs_main(Input input)
{
	Output output;
	
	input.pos = mul(world, input.pos);
	
	output.svpos = mul(mul(proj, view), input.pos);
	output.pos = mul(view, input.pos);
	
	output.normal = mul(world, input.normal); //法線にもワールド変換を行う
	output.vnormal = mul(view, output.normal);
	output.uv = input.uv;
	output.ray = normalize(input.pos.xyz - mul(view, eye)); //視線ベクトル

	return output;
}

float4 ps_main(Output input) : SV_TARGET
{	
	float3 light = normalize(float3(1, -1, 1)); //光の向かうベクトル(平行光線)
	float3 lightColor = float3(1, 1, 1); //ライトのカラー(1,1,1で真っ白)

	//ディフューズ計算
	float diffuseB = saturate(dot(-light, input.normal));
	float4 toonDif = toon.Sample(smpToon, float2(0, 1.0 - diffuseB));
	
	//光の反射ベクトル
	float3 refLight = normalize(reflect(light, input.normal.xyz));
	float specularB = pow(saturate(dot(refLight, -input.ray)), specular.a);
	
	//スフィアマップ用UV
	float2 sphereMapUV = input.vnormal.xy;
	sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);

	float4 texColor = tex.Sample(smp, input.uv); //テクスチャカラー

	float4 color =  saturate(toonDif //輝度(トゥーン)
		* diffuse //ディフューズ色
		* texColor //テクスチャカラー
		* sph.Sample(smp, sphereMapUV)) //スフィアマップ(乗算)
		+ saturate(spa.Sample(smp, sphereMapUV) * texColor //スフィアマップ(加算)
		+ float4(specularB * specular.rgb, 1)) //スペキュラー
		+ float4(texColor * ambient * 0.5, 1); //アンビエント(明るくなりすぎるので0.5にしてます)
	
#ifdef HAS_SRV_6
	color = float4(1, 0, 0, 1);
#endif
	
	return color;
}