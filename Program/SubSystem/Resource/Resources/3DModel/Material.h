/**
* @file    Material.h
* @brief
*
* @date	   2022/09/06 2022年度初版
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
	SUB_CLASS(Material)
public:

	/** asset ファイルと独自データの作成を行う */
	static Material* Create(StringView name, const ProprietaryMaterialData& data = ProprietaryMaterialData()) noexcept;

	bool Load(StringView path) override;

	/** 主にEditer上で使用される asset や独自データの更新処理 */
	void Update() override;

	void Render() noexcept;

	/** パラメーター */
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

	/** マテリアル設定の Define をシェーダーに定義 */
	void DefineSettingToShader() noexcept;

	/** セットされたシェーダファイルから各パラメーター取得 */
	void ParametricAnalysis(bool isClear = true) noexcept;

	/** 2022/08/24時点では決められたサンプラーを複数生成している */
	bool CreateSampler() noexcept;

	/** 汎用的な生成にしているためコードの改変の必要はない */
	bool CreateRootSinature() noexcept;

	/** 基本的には汎用的なコードだが、RendererTargetの数はパイプラインによって書き換える必要がある */
	bool CreatePipeline() noexcept;

	/** マテリアル用定数バッファの作成 */
	bool CreateConstantBuffer() noexcept;

	/** 設定中のシェーダーコンパイルを行う。*/
	void CompileShader() noexcept;

	/** GPU側のマテリアル設定の更新を行う */
	void UpdateConstantBufferData() noexcept;

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryMaterialData m_materialData;

	// * パラメーター名からテクスチャの切り替えが出来るように Map を使用
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