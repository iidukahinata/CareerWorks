/**
* @file    Light.cpp
* @brief
*
* @date	   2022/11/04 2022”N“x‰”Å
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
	ASSERT(m_renderer);

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

	if (GetActive())
	{
		RegisterToLightMap();
	}
}

void Light::OnUnRegister()
{
	IComponent::OnUnRegister();

	if (GetActive())
	{
		UnRegisterFromLightMap();
	}
}

void Light::OnRemove()
{
	if (m_isRegister)
	{
		UnRegisterFromLightMap();

		m_renderCommandFance.BegineFrame();
	}

	IComponent::OnRemove();
}

bool Light::Erasable()
{
	return m_renderCommandFance.IsSingle();
}

void Light::SetLightType(LightType lightType) noexcept
{
	if (IsInRenderingThread())
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
	if (IsInRenderingThread())
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
	if (IsInRenderingThread())
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
	if (IsInRenderingThread())
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
	if (IsInRenderingThread())
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
	if (m_isRegister)
	{
		return;
	}

	m_isRegister = true;

	if (IsInRenderingThread())
	{
		m_renderer->AddLight(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->AddLight(this); });
	}
}

void Light::UnRegisterFromLightMap() noexcept
{
	if (!m_isRegister)
	{
		return;
	}

	m_isRegister = false;

	if (IsInRenderingThread())
	{
		m_renderer->RemoveLight(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->RemoveLight(this); });
	}
}