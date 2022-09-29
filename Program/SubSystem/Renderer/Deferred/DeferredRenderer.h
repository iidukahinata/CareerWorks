/**
* @file	   DeferredRenderer.h
* @brief
*
* @date	   2022/09/17 2022�N�x����
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
	WAIT_FOR_DEVELOPMENT("�쐬�������ȒP��GUI�쐬��ɕύX")
	SUB_CLASS(DeferredRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	/** Render */
	void Update() noexcept;
	void Present() noexcept;

private:

	/** Render Job �̓o�^ */
	void RegisterRenderJob() noexcept;

	/** RootSignature �Ȃǂ̋��ʓI�� Rendering Object �̐��� */
	bool SetUpRenderingObjects(UINT width, UINT height) noexcept;

	/** Lighting Pass �Ŏg�p�����p�C�v���C�������� */
	bool SetUpLightingObjects(UINT width, UINT height) noexcept;

	/** PostProcess Pass �Ŏg�p�����p�C�v���C�������� */
	bool SetUpPostProcessObjects(UINT width, UINT height) noexcept;

private:

	/** Z�o�b�t�@�ALightMap��� �Ȃǂ̐��� */
	void PrePass() noexcept;

	/** PrePass �Ő���������񂩂� GBuffer �̐��� */
	void GBufferPass() noexcept;

	/** ���ۂ� Lighting ���l�������v�Z */
	void LightingPass() noexcept;

	/** Blur : DOF �Ȃǂ� Effect ���� */
	void PostPass() noexcept;

private:

	UniquePtr<GBuffer> m_gbuffer;

	// * renderTarget objects
	D3D12RenderTexture m_lightingRenderTexture;

	// * rendering objects
	D3D12Sampler m_sampler;
	D3D12RootSignature m_rootSignature;

	// * pipeline objects
	D3D12GraphicsPipelineState m_deferredPipeline;
	D3D12GraphicsPipelineState m_postProcessPipeline;

	// * sprite mesh buffer
	D3D12IndexBuffer m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	// * Update �� Present
	Array<Job, 2> m_jobs;
};