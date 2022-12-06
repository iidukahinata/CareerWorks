/**
* @file    Material.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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
* Material Load に関して 2022/8/31 時点で、Hlsl を使用した際の ShaderCompile 処理が高価すぎる
*/
class Material : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Material, IResource)
public:

	/** asset ファイルと独自データの作成を行う */
	static Material* Create(StringView name, const ProprietaryMaterialData& data = ProprietaryMaterialData()) noexcept;

	/** 独自データ読み込み及び、各パラメータのセットアップを行う */
	bool Load(StringView path) override;

	/** 主にEditer上で使用される asset や独自データの更新処理 */
	void Update() override;

	void Render() noexcept;

public:

	/** パラメーター */
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

	/** マテリアル設定の Define をシェーダーに定義 */
	bool DefineSettingToPixelShader(StringView path = String(), bool recompile = false) noexcept;
	bool DefineSettingToVertexShader(StringView path = String(), bool recompile = false) noexcept;

	/** セットされたシェーダファイルから各パラメーター取得 */
	void ParametricAnalysis(bool isClear = true) noexcept;

	/** 汎用的な生成にしているためコードの改変の必要はない */
	bool CreateRootSinature() noexcept;

	/** 基本的には汎用的なコードだが、RendererTargetの数はパイプラインによって書き換える必要がある */
	bool CreatePipeline() noexcept;

	/** マテリアル用定数バッファの作成 */
	bool CreateConstantBuffer() noexcept;

	/** 設定中のシェーダーコンパイルを行う。*/
	bool CompileShader() noexcept;

	/** GPU側のマテリアル設定の更新を行う */
	void UpdateConstantBufferData() noexcept;

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryMaterialData m_materialData;

	// * パラメーター名からテクスチャの切り替えが出来るように Map を使用
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