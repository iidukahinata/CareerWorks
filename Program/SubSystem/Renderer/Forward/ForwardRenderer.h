/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/09/16 2022年度初版
*/
#pragma once


#include "../Renderer.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"

class ForwardRenderer : public Renderer
{
	IN_DEVELOPMENT("別スレッド上での動作を開発予定")
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

private:

	// * Update と Present
	Array<Job, 2> m_jobs;

#if IS_EDITOR

	// * SceneView 描画用最終フレームを保持
	D3D12RenderTexture m_renderTexture;
#endif // IS_EDITOR
};