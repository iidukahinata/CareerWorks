/**
* @file    DefaultLightMap.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../ILightMap.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

#define MAX_LIGHT_COUNT 256

class DefaultLightMap : public ILightMap
{
	SUB_CLASS(DefaultLightMap, ILightMap)
public:

	void Initialize() override;
	void Update(ICamera* mainCamera) override;

private:

	struct DirectionalLightDate
	{
		Math::Vector4 direction;
		Math::Vector4 color; // w intensity
	};

	struct PointLightDate
	{
		Math::Vector4 position;
		Math::Vector4 color;
		float intensity;
		float influenceRange;

		Math::Vector2 padding; // 16ビット調整用変数
	};

	struct SpotLightDate
	{
		Math::Vector4 position;
		Math::Vector4 direction;
		Math::Vector4 color;
		float intensity;
		float influenceRange;
		float angle;

		float padding; // 16ビット調整用変数
	};

	struct ConstantBufferLight
	{
		Math::Vector4 eyePos;
		Math::Vector4 ambientLight;
		DirectionalLightDate directionalLight;
		Array<PointLightDate, MAX_LIGHT_COUNT> pointLights;
		Array<SpotLightDate, MAX_LIGHT_COUNT> spotLights;

		float pointLightCount;
		float spotLightCount;
	};

private:

	/** LightListからGPU転送用データを作成し、戻り値として返します。*/
	ConstantBufferLight CreateConstantBufferLight(const ICamera* mainCamera) const noexcept;

private:

	D3D12ConstantBuffer m_constantBuffer;
};