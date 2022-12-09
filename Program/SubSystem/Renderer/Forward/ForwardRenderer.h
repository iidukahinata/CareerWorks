/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IRenderer.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"

class ForwardRenderer : public IRenderer
{
	IN_DEVELOPMENT("カリング処理を開発予定")
	SUB_CLASS(ForwardRenderer, IRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	/** Render */
	void Update() noexcept;
	void Present() noexcept;

private:

	/** Render Job の登録 */
	void RegisterRenderJob() noexcept;

	/** パイプラインで使用する全てのオブジェクトの初期化 */
	bool SetupObjects() noexcept;

	/** Z prepass で使用されるオブジェクトの初期化 */
	bool SetupPrePassObjects() noexcept;

private:

	/** 実際に描画するオブジェクトのバッチ生成処理 */
	void CreateRenderList();

	/** フレーム内で使用される Buffer の更新及び、Z prepass などの描画時の最適化処理を行う。*/
	void PrePass() noexcept;

	/** Zバッファ生成 */
	void ZPrePass();

	/** PrePass で作成した情報から通常レンダリングを行う。*/
	void LightingPass() noexcept;

private:

	// * Update と Present
	Array<Job, 2> m_jobs;

#if IS_EDITOR

	// * SceneView 描画用最終フレームを保持
	D3D12RenderTexture m_renderTexture;

#endif // IS_EDITOR

	// * rendering objects
	D3D12RootSignature m_preZrootSignature;

	// * pipeline objects
	D3D12GraphicsPipelineState m_preZPipeline;
	D3D12GraphicsPipelineState m_instancePreZPipeline;

public:

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

	struct DrawBacth
	{
		Material* material;
		D3D12ConstantBuffer* constantBuffer;
		Vector<D3D12VertexBuffer*> vertexBuffers;
		D3D12IndexBuffer* indexBuffer;
		uint32_t indexNum;
		uint32_t instancingNum;
	};

	struct DrawInstancingBacth : public DrawBacth
	{
		D3D12VertexBuffer instancingBuffer;
		Vector<DirectX::XMMATRIX> matList;
	};

	Vector<DrawBacth> m_bacthList;
	Vector<D3D12VertexBuffer> m_instancingBuffer;
};