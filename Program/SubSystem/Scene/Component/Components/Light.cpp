/**
* @file    Light.cpp
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/


#include "Light.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void Light::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	file->Write(m_lightType);
	file->Write(m_color);
	file->Write(m_intensity);
	file->Write(m_influenceRange);
	file->Write(m_angle);
}

void Light::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

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
	if (IsRenderingThread())
	{
		m_lightType = lightType;
	}
	else
	{
		RegisterRenderCommand([this, lightType] { m_lightType = lightType; });
	}
}

LightType Light::GetLightType() const noexcept
{
	return m_lightType;
}

void Light::SetColor(const Math::Vector4& color) noexcept
{
	if (IsRenderingThread())
	{
		m_color = color;
	}
	else
	{
		RegisterRenderCommand([this, color] { m_color = color; });
	}
}

const Math::Vector4& Light::GetColor() const noexcept
{
	return m_color;
}

void Light::SetIntensity(float intensity) noexcept
{
	if (IsRenderingThread())
	{
		m_intensity = intensity;
	}
	else
	{
		RegisterRenderCommand([this, intensity] { m_intensity = intensity; });
	}
}

float Light::GetIntensity() const noexcept
{
	return m_intensity;
}

void Light::SetInfluenceRange(float influenceRange) noexcept
{
	if (IsRenderingThread())
	{
		m_influenceRange = influenceRange;
	}
	else
	{
		RegisterRenderCommand([this, influenceRange] { m_influenceRange = influenceRange; });
	}
}

float Light::GetInfluenceRange() const noexcept
{
	return m_influenceRange;
}

void Light::SetAngle(float angle) noexcept
{
	if (IsRenderingThread())
	{
		m_angle = angle;
	}
	else
	{
		RegisterRenderCommand([this, angle] { m_angle = angle; });
	}
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