/**
* @file    Material.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "Shader.h"
#include "Texture.h"
#include "../IResource.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12Sampler.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12RootSignature.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"
#include "SubSystem/Resource/ResourceData/ProprietaryMaterialData.h"

struct TexturePramInfo
{
	uint32_t m_bindPoint = 0;
	Texture* m_texture = nullptr;
};

/**
* @note
* Material Load �Ɋւ��� 2022/8/31 ���_�ŁAHlsl ���g�p�����ۂ� ShaderCompile ����������������
*/
class Material : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Material)
public:

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s�� */
	static Material* Create(StringView name, const ProprietaryMaterialData& data = ProprietaryMaterialData()) noexcept;

	bool Load(StringView path) override;

	/** ���Editer��Ŏg�p����� asset ��Ǝ��f�[�^�̍X�V���� */
	void Update() override;

	void Render() noexcept;

	/** �p�����[�^�[ */
	void SetBlendMode(BLEND_MODE mode) noexcept;
	BLEND_MODE GetBlendMode() const noexcept;
	void SetRasterizerState(RASTERIZER_STATE mode) noexcept;
	RASTERIZER_STATE GetRasterizerState() const noexcept;
	void SetShader(ShaderType type, StringView name, bool createPipeline = true) noexcept;
	void SetTexture(StringView pramName, Texture* texture, bool isDefineToShader = false) noexcept;
	const Unordered_Map<String, TexturePramInfo>& GetTextures() const noexcept;
	void SetDiffuse(const Math::Vector3& diffuse) noexcept;
	const Math::Vector3& GetDiffuse() const noexcept;
	void SetSpecular(const Math::Vector3& specular) noexcept;
	const Math::Vector3 GetSpecular() const noexcept;
	void SetSpecularPower(float specularPower) noexcept;
	float GetSpecularPower() noexcept;
	void SetAmbient(const Math::Vector3& ambient) noexcept;
	const Math::Vector3& GetAmbient() const noexcept;
	void SetEmissive(const Math::Vector3& emissive) noexcept;
	const Math::Vector3 GetEmissive() const noexcept;

	bool IsTranslucent() const noexcept;

private:

	/** �}�e���A���ݒ�� Define ���V�F�[�_�[�ɒ�` */
	void DefineSettingToShader() noexcept;

	/** �Z�b�g���ꂽ�V�F�[�_�t�@�C������e�p�����[�^�[�擾 */
	void ParametricAnalysis(bool isClear = true) noexcept;

	/** 2022/08/24���_�ł͌��߂�ꂽ�T���v���[�𕡐��������Ă��� */
	bool CreateSampler() noexcept;

	/** �ėp�I�Ȑ����ɂ��Ă��邽�߃R�[�h�̉��ς̕K�v�͂Ȃ� */
	bool CreateRootSinature() noexcept;

	/** ��{�I�ɂ͔ėp�I�ȃR�[�h�����ARendererTarget�̐��̓p�C�v���C���ɂ���ď���������K�v������ */
	bool CreatePipeline() noexcept;

	/** �}�e���A���p�萔�o�b�t�@�̍쐬 */
	bool CreateConstantBuffer() noexcept;

	/** �ݒ蒆�̃V�F�[�_�[�R���p�C�����s���B*/
	void CompileShader() noexcept;

	/** GPU���̃}�e���A���ݒ�̍X�V���s�� */
	void UpdateConstantBufferData() noexcept;

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryMaterialData m_materialData;

	// * �p�����[�^�[������e�N�X�`���̐؂�ւ����o����悤�� Map ���g�p
	Unordered_Map<String, TexturePramInfo> m_textures;

	Shader m_shader;

	Vector<D3D12Sampler> m_samplers;
	D3D12GraphicsPipelineState m_pipeline;
	D3D12RootSignature m_rootSignature;

	struct ConstantBufferMaterial
	{
		Math::Vector3 diffuse;
		float alpha;
		Math::Vector3 specular;
		float specularPower;
		Math::Vector3 ambient;
	};

	D3D12ConstantBuffer m_constantBufferMaterial;
};