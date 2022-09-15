
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
	float4 svpos:SV_POSITION;//�V�X�e���p���_���W
	float4 pos:POSITION;//�V�X�e���p���_���W
	float4 normal:NORMAL0;//�@���x�N�g��
	float4 vnormal:NORMAL1;//�@���x�N�g��
	float2 uv:TEXCOORD;//UV�l
	float3 ray:VECTOR;//�x�N�g��
};

Output vs_main(Input input)
{
	Output output;
	
	input.pos = mul(world, input.pos);
	
	output.svpos = mul(mul(proj, view), input.pos);
	output.pos = mul(view, input.pos);
	
	output.normal = mul(world, input.normal); //�@���ɂ����[���h�ϊ����s��
	output.vnormal = mul(view, output.normal);
	output.uv = input.uv;
	output.ray = normalize(input.pos.xyz - mul(view, eye)); //�����x�N�g��

	return output;
}

float4 ps_main(Output input) : SV_TARGET
{	
	float3 light = normalize(float3(1, -1, 1)); //���̌������x�N�g��(���s����)
	float3 lightColor = float3(1, 1, 1); //���C�g�̃J���[(1,1,1�Ő^����)

	//�f�B�t���[�Y�v�Z
	float diffuseB = saturate(dot(-light, input.normal));
	float4 toonDif = toon.Sample(smpToon, float2(0, 1.0 - diffuseB));
	
	//���̔��˃x�N�g��
	float3 refLight = normalize(reflect(light, input.normal.xyz));
	float specularB = pow(saturate(dot(refLight, -input.ray)), specular.a);
	
	//�X�t�B�A�}�b�v�pUV
	float2 sphereMapUV = input.vnormal.xy;
	sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);

	float4 texColor = tex.Sample(smp, input.uv); //�e�N�X�`���J���[

	float4 color =  saturate(toonDif //�P�x(�g�D�[��)
		* diffuse //�f�B�t���[�Y�F
		* texColor //�e�N�X�`���J���[
		* sph.Sample(smp, sphereMapUV)) //�X�t�B�A�}�b�v(��Z)
		+ saturate(spa.Sample(smp, sphereMapUV) * texColor //�X�t�B�A�}�b�v(���Z)
		+ float4(specularB * specular.rgb, 1)) //�X�y�L�����[
		+ float4(texColor * ambient * 0.5, 1); //�A���r�G���g(���邭�Ȃ肷����̂�0.5�ɂ��Ă܂�)
	
#ifdef HAS_SRV_6
	color = float4(1, 0, 0, 1);
#endif
	
	return color;
}