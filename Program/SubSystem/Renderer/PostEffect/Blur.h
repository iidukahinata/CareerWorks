/**
* @file    Blur.h
* @brief
*
* @date	   2022/10/29 2022年度初版
*/
#pragma once


#include "PostEffect.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Blur : public PostEffect
{
	IN_DEVELOPMENT()
		SUB_CLASS(Blur)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void Initialize() override;

	void Render() override;

	void SetVariance(float variance) noexcept;
	float GetVariance() const noexcept;

private:

	/** 重み付けテーブルの作成をして Buffer の更新を行う。*/
	void CalculateWeightTablesFromGaussian() noexcept;

private:

	float m_variance;

	struct ConstantBufferBlur
	{
		float weights[8];
		Math::Vector2 screenSize;
	};

	D3D12ConstantBuffer m_constantBuffer;
};