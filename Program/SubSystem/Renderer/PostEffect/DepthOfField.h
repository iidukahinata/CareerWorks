/**
* @file    DepthOfField.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "IPostEffect.h"
#include "../GraphicsAPI/D3D12/D3D12Sampler.h"
#include "../GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12VertexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class DepthOfField : public IPostEffect
{
	IN_DEVELOPMENT()
	SUB_CLASS(DepthOfField, IPostEffect)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void Initialize() override;

	void Render() override;

	D3D12RenderTexture& GetTexture() override;

private:

	D3D12RenderTexture m_renderTexture;
};