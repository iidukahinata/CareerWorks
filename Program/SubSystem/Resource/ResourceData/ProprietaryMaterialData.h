/**
* @file    ProprietaryMaterialData.h
* @brief
*
* @date	   2022/09/06 2022年度初版
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

	// * マテリアルで使用するマテリアル名
	Unordered_Map<String, TexturePramInfo> m_textureInfos;

	// * マテリアルで使用するシェーダー名
	Array<String, ShaderType::NumAllType> m_shaderPaths;

public:

	ProprietaryMaterialData();

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	/** assimp データから独自データに変換、テクスチャ生成で使用 */
	static ProprietaryMaterialData ConvertProprietaryData(aiMaterial* material) noexcept;
	/** pmd データから独自データに変換、テクスチャ生成で使用 */
	static ProprietaryMaterialData ConvertProprietaryData(PMDMaterial* material) noexcept;

	/** 独自データとして保存される出力先ファイルパスに変換 */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};