/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/09/16 2022�N�x����
*/
#pragma once


#include "../Renderer.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"

class ForwardRenderer : public Renderer
{
	IN_DEVELOPMENT("�ʃX���b�h��ł̓�����J���\��")
	SUB_CLASS(ForwardRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	/** Render */
	void Update() noexcept;
	void Present() noexcept;

private:

	/** Render Job �̓o�^ */
	void RegisterRenderJob() noexcept;

private:

	// * Update �� Present
	Array<Job, 2> m_jobs;

#if IS_EDITOR

	// * SceneView �`��p�ŏI�t���[����ێ�
	D3D12RenderTexture m_renderTexture;
#endif // IS_EDITOR
};