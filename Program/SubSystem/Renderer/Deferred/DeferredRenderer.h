/**
* @file	   DeferredRenderer.h
* @brief
*
* @date	   2022/11/27 2022�N�x����
*/
#pragma once


#include "GBuffer.h"
#include "../IRenderer.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "../GraphicsAPI/D3D12/D3D12RootSignature.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12Sampler.h"
#include "../GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12VertexBuffer.h"

class DeferredRenderer : public IRenderer
{
	COMPLETED_DEVELOPMENT()
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

	/** �p�C�v���C���Ŏg�p����S�ẴI�u�W�F�N�g�̏����� */
	bool SetupObjects() noexcept;

	/** Z prepass �Ŏg�p�����I�u�W�F�N�g�̏����� */
	bool SetupPrePassObjects() noexcept;

	/** RootSignature �Ȃǂ̋��ʓI�� Rendering Object �̐��� */
	bool SetupRenderingObjects(UINT width, UINT height) noexcept;

	/** Lighting Pass �Ŏg�p�����p�C�v���C�������� */
	bool SetupLightingObjects(UINT width, UINT height) noexcept;

	/** PostProcess Pass �Ŏg�p�����p�C�v���C�������� */
	bool SetupPostProcessObjects(UINT width, UINT height) noexcept;

private:

	/** �t���[�����Ŏg�p����`����̐��� */
	void PrePass() noexcept;

	/** Z�o�b�t�@���� */
	void ZPrePass();

	/** PrePass �Ő���������񂩂� GBuffer �̐��� */
	void GBufferPass() noexcept;

	/** Lighting �O���� */
	void PreLightingPass() noexcept;

	/** ���ۂ� Lighting ���l�������v�Z */
	void LightingPass() noexcept;

	/** Bloom : DOF �Ȃǂ� Effect ���� */
	void PostPass() noexcept;

	/** �P�x�e�N�X�`���̐��� */
	void LuminousPass() noexcept;

private:

	UniquePtr<GBuffer> m_gbuffer;

	// * renderTarget objects
	D3D12RenderTexture m_lightingRenderTexture;
	D3D12RenderTexture m_luminousRenderTexture;

#if IS_EDITOR

	// * SceneView �`��p�ŏI�t���[����ێ�
	D3D12RenderTexture m_renderTexture;

#endif // IS_EDITOR

	// * rendering objects
	D3D12Sampler m_sampler;
	D3D12RootSignature m_rootSignature;
	D3D12RootSignature m_preZrootSignature;

	// * pipeline objects
	D3D12GraphicsPipelineState m_preZPipeline;
	D3D12GraphicsPipelineState m_deferredPipeline;
	D3D12GraphicsPipelineState m_luminousPipeline;
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
	Math::Matrix m_camera2DView;
	Math::Matrix m_camera2DProj;
	D3D12ConstantBuffer m_constantBuffer;

	// * sprite mesh buffer
	D3D12IndexBuffer m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	// * Update �� Present
	Array<Job, 2> m_jobs;
};