/**
* @file    Light.cpp
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/


#include "Light.h"
#include "SubSystem/Renderer/Renderer.h"

void Light::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	file->Write(m_lightType);
	file->Write(m_color);
	file->Write(m_intensity);
	file->Write(m_influenceRange);
	file->Write(m_angle);
}

void Light::Deserialization(FileStream* file)
{
	IComponent::Deserialization(file);

	file->Read(&m_lightType);
	file->Read(&m_color);
	file->Read(&m_intensity);
	file->Read(&m_influenceRange);
	file->Read(&m_angle);
}

void Light::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();

	// init data
	m_lightType = LightType::DirectionalLight;
	m_color = Math::Vector4(1, 1, 1, 1);
	m_angle = 0;
	m_intensity = 1;
	m_influenceRange = 5;

	GetTransform().SetRotation(Math::Vector3(0.5f, -0.5f, -1.f));
}

void Light::OnRegister()
{
	IComponent::OnRegister();

	RegisterToLightMap();
}

void Light::OnUnRegister()
{
	IComponent::OnUnRegister();

	UnRegisterFromLightMap();
}

void Light::SetLightType(LightType lightType) noexcept
{
	m_lightType = lightType;
}

LightType Light::GetLightType() const noexcept
{
	return m_lightType;
}

void Light::SetColor(const Math::Vector4& color) noexcept
{
	m_color = color;
}

const Math::Vector4& Light::GetColor() const noexcept
{
	return m_color;
}

void Light::SetIntensity(float intensity) noexcept
{
	m_intensity = intensity;
}

float Light::GetIntensity() const noexcept
{
	return m_intensity;
}

void Light::SetInfluenceRange(float influenceRange) noexcept
{
	m_influenceRange = influenceRange;
}

float Light::GetInfluenceRange() const noexcept
{
	return m_influenceRange;
}

void Light::SetAngle(float angle) noexcept
{
	m_angle = angle;
}

float Light::GetAngle() const noexcept
{
	return m_angle;
}

void Light::RegisterToLightMap() noexcept
{
	ASSERT(m_renderer);
	m_renderer->AddLight(this);
}

void Light::UnRegisterFromLightMap() noexcept
{
	ASSERT(m_renderer);
	m_renderer->RemoveLight(this);
}