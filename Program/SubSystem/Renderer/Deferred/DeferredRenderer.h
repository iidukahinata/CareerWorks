/**
* @file	   DeferredRenderer.h
* @brief
*
* @date	   2022/09/02 2022年度初版
*/
#pragma once


#include "../Renderer.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "../GraphicsAPI/D3D12/D3D12RootSignature.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12Sampler.h"
#include "../GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12VertexBuffer.h"

class GBuffer;

class DeferredRenderer : public Renderer
{
	WAIT_FOR_DEVELOPMENT("作成順序を簡単なGUI作成後に変更")
	SUB_CLASS(DeferredRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	void Update() noexcept;

private:

	bool SetUpDeferredObjects(UINT width, UINT height) noexcept;
	bool SetUpSpriteObjects(UINT width, UINT height) noexcept;

private:

	UniquePtr<GBuffer> m_gbuffer;

	// * shader objects
	Array<D3D12Shader, 2> m_deferredShaders;
	Array<D3D12Shader, 2> m_spriteShaders;

	// * sprite mesh
	D3D12IndexBuffer m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	D3D12RenderTexture m_renderTexture;
	D3D12Sampler m_sampler;

	Job m_job;
};