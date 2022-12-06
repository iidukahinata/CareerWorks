/**
* @file    Material.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
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
	SUB_CLASS(Material, IResource)
public:

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s�� */
	static Material* Create(StringView name, const ProprietaryMaterialData& data = ProprietaryMaterialData()) noexcept;

	/** �Ǝ��f�[�^�ǂݍ��݋y�сA�e�p�����[�^�̃Z�b�g�A�b�v���s�� */
	bool Load(StringView path) override;

	/** ���Editer��Ŏg�p����� asset ��Ǝ��f�[�^�̍X�V���� */
	void Update() override;

	void Render() noexcept;

public:

	/** �p�����[�^�[ */
	void SetBlendMode(BLEND_MODE mode) noexcept;
	BLEND_MODE GetBlendMode() const noexcept;
	bool IsTranslucent() const noexcept;
	void SetIsInstancing(bool instancing) noexcept;
	bool IsInstancing() const noexcept;
	void SetRasterizerState(RASTERIZER_STATE mode) noexcept;
	RASTERIZER_STATE GetRasterizerState() const noexcept;
	bool SetShader(ShaderType type, StringView path, bool createPipeline = true) noexcept;
	void RefreshShader() noexcept;
	const Array<String, ShaderType::NumAllType>& GetShaderPaths() const noexcept;
	void SetTexture(StringView pramName, Texture* texture, bool isDefineToShader = false) noexcept;
	void AddTexturePram(StringView pramName, uint32_t bindPoint) noexcept;
	const Unordered_Map<String, TexturePramInfo>& GetTextures() const noexcept;
	void SetAlbedo(const Math::Vector3& albedo) noexcept;
	const Math::Vector3& GetAlbedo() const noexcept;
	void SetMetallic(float metallic) noexcept;
	float GetMetallic() const noexcept;
	void SetSmooth(float smooth) noexcept;
	float GetSmooth() noexcept;
	void SetEmission(const Math::Vector3& emissive) noexcept;
	const Math::Vector3& GetEmission() const noexcept;

private:

	/** �}�e���A���ݒ�� Define ���V�F�[�_�[�ɒ�` */
	bool DefineSettingToPixelShader(StringView path = String(), bool recompile = false) noexcept;
	bool DefineSettingToVertexShader(StringView path = String(), bool recompile = false) noexcept;

	/** �Z�b�g���ꂽ�V�F�[�_�t�@�C������e�p�����[�^�[�擾 */
	void ParametricAnalysis(bool isClear = true) noexcept;

	/** �ėp�I�Ȑ����ɂ��Ă��邽�߃R�[�h�̉��ς̕K�v�͂Ȃ� */
	bool CreateRootSinature() noexcept;

	/** ��{�I�ɂ͔ėp�I�ȃR�[�h�����ARendererTarget�̐��̓p�C�v���C���ɂ���ď���������K�v������ */
	bool CreatePipeline() noexcept;

	/** �}�e���A���p�萔�o�b�t�@�̍쐬 */
	bool CreateConstantBuffer() noexcept;

	/** �ݒ蒆�̃V�F�[�_�[�R���p�C�����s���B*/
	bool CompileShader() noexcept;

	/** GPU���̃}�e���A���ݒ�̍X�V���s�� */
	void UpdateConstantBufferData() noexcept;

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryMaterialData m_materialData;

	// * �p�����[�^�[������e�N�X�`���̐؂�ւ����o����悤�� Map ���g�p
	Unordered_Map<String, TexturePramInfo> m_textureInfos;

	Shader m_shader;

	Map<UINT, D3D12Sampler> m_samplers;
	D3D12RootSignature m_rootSignature;
	D3D12GraphicsPipelineState m_pipeline;

	struct ConstantBufferMaterial
	{
		Math::Vector3 albedo;
		float metallic;
		float smooth;
		Math::Vector3 emission;
	};

	D3D12ConstantBuffer m_constantBufferMaterial;
};