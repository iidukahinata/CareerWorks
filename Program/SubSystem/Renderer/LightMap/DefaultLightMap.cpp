/**
* @file    DefaultLightMap.cpp
* @brief
*
* @date	   2022/09/19 2022年度初版
*/


#include "DefaultLightMap.h"
#include "SubSystem/Scene/Component/Components/Light.h"
#include "SubSystem/Scene/Component/Components/Camera.h"

void DefaultLightMap::Initialize()
{
	m_ambientLight = Math::Vector4(0.5f);

	m_constantBuffer.Create(sizeof(ConstantBufferLight));
}

void DefaultLightMap::Update(Camera* mainCamera)
{
	// pre update
	ConstantBufferLight buffer = CreateConstantBufferLight(mainCamera);

	// update buffer
	m_constantBuffer.Update(buffer);

	// set buffer
	m_constantBuffer.VSSet(1);
}

DefaultLightMap::ConstantBufferLight DefaultLightMap::CreateConstantBufferLight(const Camera* mainCamera) const noexcept
{
	ConstantBufferLight buffer = {};

	int pointLightCount = 0;
	int spotLightCount = 0;

	// 各 Light 情報を取得し、buffer にセット
	for (auto light : m_lights)
	{
		switch (light->GetLightType())
		{
		case LightType::DirectionalLight:
			buffer.directionalLight.direction = Math::Vector4(light->GetTransform().GetRotation(), 0.f);
			buffer.directionalLight.color	  = light->GetColor();
			buffer.directionalLight.color.w   = light->GetIntensity();
			break;

		case LightType::PointLight:
			buffer.pointLights[pointLightCount].position	   = Math::Vector4(light->GetTransform().GetPosition(), 0.f);
			buffer.pointLights[pointLightCount].color		   = light->GetColor();
			buffer.pointLights[pointLightCount].intensity	   = light->GetIntensity();
			buffer.pointLights[pointLightCount].influenceRange = light->GetInfluenceRange();
			++pointLightCount;
			break;

		case LightType::SpotLight:
			buffer.spotLights[spotLightCount].position		 = Math::Vector4(light->GetTransform().GetPosition(), 0.f);
			buffer.spotLights[spotLightCount].direction		 = Math::Vector4(light->GetTransform().GetRotation(), 0.f);
			buffer.spotLights[spotLightCount].color			 = light->GetColor();
			buffer.spotLights[spotLightCount].intensity		 = light->GetIntensity();
			buffer.spotLights[spotLightCount].influenceRange = light->GetInfluenceRange();
			buffer.spotLights[spotLightCount].angle			 = light->GetAngle();
			++spotLightCount;
			break;

		default:
			break;
		}
	}

	// 他 Light データを設定
	buffer.eyePos		   = Math::Vector4(mainCamera->GetTransform().GetPosition(), 0);
	buffer.ambientLight    = m_ambientLight;
	buffer.pointLightCount = pointLightCount;
	buffer.spotLightCount  = spotLightCount;

	return buffer;
}