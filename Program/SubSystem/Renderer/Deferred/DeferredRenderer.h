/**
* @file	   DeferredRenderer.h
* @brief
*
* @date	   2022/11/03 2022年度初版
*/
#pragma once


#include "GBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "../GraphicsAPI/D3D12/D3D12RootSignature.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12Sampler.h"
#include "../GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12VertexBuffer.h"

class DeferredRenderer : public Renderer
{
	WAIT_FOR_DEVELOPMENT("作成順序を簡単なGUI作成後に変更")
	SUB_CLASS(DeferredRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	/** Render */
	void Update() noexcept;
	void Present() noexcept;

	void RegisterGBufferShader(StringView path) override;
	String GetGBufferShader() override;

private:

	/** Render Job の登録 */
	void RegisterRenderJob() noexcept;

	/** Z prepass で使用されるオブジェクトの初期化 */
	bool SetUpPrePassObjects() noexcept;

	/** RootSignature などの共通的な Rendering Object の生成 */
	bool SetUpRenderingObjects(UINT width, UINT height) noexcept;

	/** Lighting Pass で使用されるパイプライン初期化 */
	bool SetUpLightingObjects(UINT width, UINT height) noexcept;

	/** PostProcess Pass で使用されるパイプライン初期化 */
	bool SetUpPostProcessObjects(UINT width, UINT height) noexcept;

private:

	/** Zバッファ、LightMap情報 などの生成 */
	void PrePass() noexcept;

	/** PrePass で生成した情報から GBuffer の生成 */
	void GBufferPass() noexcept;

	/** Lighting 前処理 */
	void PreLightingPass() noexcept;

	/** 実際に Lighting を考慮した計算 */
	void LightingPass() noexcept;

	/** Bloom : DOF などの Effect 処理 */
	void PostPass() noexcept;

	/** 輝度テクスチャの生成 */
	void LuminousPass() noexcept;

private:

	UniquePtr<GBuffer> m_gbuffer;

	// * renderTarget objects
	D3D12RenderTexture m_lightingRenderTexture;
	D3D12RenderTexture m_luminousRenderTexture;

#if IS_EDITOR

	// * SceneView 描画用最終フレームを保持
	D3D12RenderTexture m_renderTexture;

#endif // IS_EDITOR

	// * rendering objects
	D3D12Sampler m_sampler;
	D3D12RootSignature m_rootSignature;
	D3D12RootSignature m_preZrootSignature;

	// * pipeline objects
	D3D12GraphicsPipelineState m_preZPipeline;
	D3D12GraphicsPipelineState m_deferredPipeline;
	D3D12GraphicsPipelineState m_postProcessPipeline;

	struct ConstantBufferMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldView;
		DirectX::XMMATRIX worldViewProjection;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX viewProjection;
		DirectX::XMMATRIX viewProjectionInverse;
		DirectX::XMMATRIX projection;
	};

	// * 2D camera objects
	Math::Matrix m_Camera2DView;
	Math::Matrix m_Camera2DProj;
	D3D12ConstantBuffer m_constantBuffer;

	// * sprite mesh buffer
	D3D12IndexBuffer m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	// * Update と Present
	Array<Job, 2> m_jobs;
};