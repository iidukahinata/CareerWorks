/**
* @file    ProprietaryMaterialData.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "ProprietaryData.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12PipelineState.h"

struct aiMaterial;
struct PMDMaterial;

struct ProprietaryMaterialData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
private:

	struct TexturePramInfo
	{
		String texturePath;
		uint32_t bindPoint;
	};

public:

	Math::Vector3 m_albedo;
	float m_metallic;
	float m_smooth;
	Math::Vector3 m_emission;

	RASTERIZER_STATE m_rasterizerState;
	BLEND_MODE m_blendMode;

	// * �}�e���A���Ŏg�p����}�e���A����
	Unordered_Map<String, TexturePramInfo> m_textureInfos;

	// * �}�e���A���Ŏg�p����V�F�[�_�[��
	Array<String, ShaderType::NumAllType> m_shaderPaths;

public:

	ProprietaryMaterialData();

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	/** assimp �f�[�^����Ǝ��f�[�^�ɕϊ��A�e�N�X�`�������Ŏg�p */
	static ProprietaryMaterialData ConvertProprietaryData(aiMaterial* material) noexcept;
	/** pmd �f�[�^����Ǝ��f�[�^�ɕϊ��A�e�N�X�`�������Ŏg�p */
	static ProprietaryMaterialData ConvertProprietaryData(PMDMaterial* material) noexcept;

	/** �Ǝ��f�[�^�Ƃ��ĕۑ������o�͐�t�@�C���p�X�ɕϊ� */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};