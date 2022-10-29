/**
* @file    Blur.cpp
* @brief
*
* @date	   2022/10/29 2022年度初版
*/


#include "Blur.h"
#include "SubSystem/Window/Window.h"

void Blur::Serialized(FileStream* file) const
{

}

void Blur::Deserialized(FileStream* file)
{

}

void Blur::Initialize()
{
	auto buffer = static_cast<ConstantBufferBlur*>(m_constantBuffer.GetCPUData());

	SetVariance(10.0f);
	buffer->screenSize.x = Window::Get().GetWindowWidth();
	buffer->screenSize.y = Window::Get().GetWindowHeight();
}

void Blur::Render()
{

}

void Blur::SetVariance(float variance) noexcept
{
	m_variance = variance;

	// 新しく重みテーブル作成
	CalculateWeightTablesFromGaussian();
}

float Blur::GetVariance() const noexcept
{
	return m_variance;
}

void Blur::CalculateWeightTablesFromGaussian() noexcept
{
	auto buffer = static_cast<ConstantBufferBlur*>(m_constantBuffer.GetCPUData());

	float weightSum = 0.f;
	for (int i = 0; i < 4; ++i)
	{
		buffer->weights[i] = expf(-0.5f * static_cast<float>(i * i) / m_variance);
		weightSum += 2.f * buffer->weights[i];
	}

	// normalize
	for (int i = 0; i < 8; ++i)
	{
		buffer->weights[i] /= weightSum;
	}
}