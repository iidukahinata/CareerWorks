/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../IRenderer.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"

class ForwardRenderer : public IRenderer
{
	IN_DEVELOPMENT("�J�����O�������J���\��")
	SUB_CLASS(ForwardRenderer, IRenderer)
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

private:

	/** ���ۂɕ`�悷��I�u�W�F�N�g�̃o�b�`�������� */
	void CreateRenderList();

	/** �t���[�����Ŏg�p����� Buffer �̍X�V�y�сAZ prepass �Ȃǂ̕`�掞�̍œK���������s���B*/
	void PrePass() noexcept;

	/** Z�o�b�t�@���� */
	void ZPrePass();

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