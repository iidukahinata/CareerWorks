/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/10/31 2022�N�x����
*/
#pragma once


#include "../Renderer.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"

class ForwardRenderer : public Renderer
{
	IN_DEVELOPMENT("�J�����O�����ƕʃX���b�h��ł̓�����J���\��")
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

	/** Z prepass �Ŏg�p�����I�u�W�F�N�g�̏����� */
	bool SetUpPrePassObjects() noexcept;

private:

	/** �t���[�����Ŏg�p����� Buffer �̍X�V�y�сAZ prepass �Ȃǂ̕`�掞�̍œK���������s���B*/
	void PrePass() noexcept;

	/** PrePass �ō쐬������񂩂�ʏ탌���_�����O���s���B*/
	void LightingPass() noexcept;

private:

	// * Update �� Present
	Array<Job, 2> m_jobs;

#if IS_EDITOR

	// * SceneView �`��p�ŏI�t���[����ێ�
	D3D12RenderTexture m_renderTexture;

#endif // IS_EDITOR

	// * rendering objects
	D3D12RootSignature m_preZrootSignature;

	// * pipeline objects
	D3D12GraphicsPipelineState m_preZPipeline;
};