/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/10/31 2022年度初版
*/
#pragma once


#include "../Renderer.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"

class ForwardRenderer : public Renderer
{
	IN_DEVELOPMENT("カリング処理と別スレッド上での動作を開発予定")
	SUB_CLASS(ForwardRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	/** Render */
	void Update() noexcept;
	void Present() noexcept;

private:

	/** Render Job の登録 */
	void RegisterRenderJob() noexcept;

	/** Z prepass で使用されるオブジェクトの初期化 */
	bool SetUpPrePassObjects() noexcept;

private:

	/** フレーム内で使用される Buffer の更新及び、Z prepass などの描画時の最適化処理を行う。*/
	void PrePass() noexcept;

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
};