/**
* @file    DefaultLightMap.h
* @brief
*
* @date	   2022/09/19 2022�N�x����
*/
#pragma once


#include "../LightMap.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

#define MAX_LIGHT_COUNT 256

class DefaultLightMap : public LightMap
{
	SUB_CLASS(DefaultLightMap)
public:

	void Initialize() override;
	void Update(Camera* mainCamera) override;

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

		Math::Vector2 padding; // 16�r�b�g�����p�ϐ�
	};

	struct SpotLightDate
	{
		Math::Vector4 position;
		Math::Vector4 direction;
		Math::Vector4 color;
		float intensity;
		float influenceRange;
		float angle;

		float padding; // 16�r�b�g�����p�ϐ�
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

	/** LightList����GPU�]���p�f�[�^���쐬���A�߂�l�Ƃ��ĕԂ��܂��B*/
	ConstantBufferLight CreateConstantBufferLight(const Camera* mainCamera) const noexcept;

private:

	D3D12ConstantBuffer m_constantBuffer;
};