/**
* @file    Camera.cpp
* @brief
*
* @date	   2022/11/04 2022”N“x‰”Å
*/


#include "Camera.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void Camera::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	file->Write(m_aspect);
	file->Write(m_width);
	file->Write(m_height);
	file->Write(m_fov);
	file->Write(m_near);
	file->Write(m_far);
}

void Camera::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	file->Read(&m_aspect);
	file->Read(&m_width);
	file->Read(&m_height);
	file->Read(&m_fov);
	file->Read(&m_near);
	file->Read(&m_far);

	// create matrix
	CreateProjectionMatrix();
	CreateOrthographicMatrix();
}

void Camera::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();
	ASSERT(m_renderer);

	// init data
	m_width  = Window::Get().GetWindowWidth();
	m_height = Window::Get().GetWindowHeight();
	m_aspect = m_width / m_height;
	m_fov	 = Math::ToRadian(45.0f);
	m_near	 = 0.1f;
	m_far	 = 1000.0f;

	// create matrix
	m_projection   = Math::Matrix::CreatePerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
	m_orthographic = Math::Matrix::CreateOrthographicLH(m_width, m_height, m_near, m_far);
}

void Camera::OnRegister()
{
	IComponent::OnRegister();

	if (GetActive())
	{
		RegisterToRenderer();
	}
}

void Camera::OnUnRegister()
{
	IComponent::OnUnRegister();

	if (GetActive())
	{
		UnRegisterFromRenderer();
	}
}

void Camera::OnRemove()
{
	if (m_isRegister)
	{
		UnRegisterFromRenderer();

		m_renderCommandFance.BegineFrame();
	}

	IComponent::OnRemove();
}

bool Camera::Erasable()
{
	return m_renderCommandFance.IsSingle();
}

float Camera::GetAspect() const noexcept
{
	return m_aspect;
}

void Camera::SetAspect(float aspect) noexcept
{
	m_aspect = aspect;
	CreateProjectionMatrix();
}

float Camera::GetWidth() const noexcept
{
	return m_width;
}

void Camera::SetWidth(float width) noexcept
{
	m_width = width;
	CreateOrthographicMatrix();
}

float Camera::GetHeight() const noexcept
{
	return m_height;
}

void Camera::SetHeight(float height) noexcept
{
	m_height = height;
	CreateOrthographicMatrix();
}

float Camera::GetFov() const noexcept
{
	return m_fov;
}

void Camera::SetFov(float fov) noexcept
{
	m_fov = fov;
	CreateProjectionMatrix();
}

float Camera::GetNear() const noexcept
{
	return m_near;
}

void Camera::SetNear(float nearClip) noexcept
{
	m_near = max(0.01f, nearClip);
	CreateProjectionMatrix();
	CreateOrthographicMatrix();
}

float Camera::GetFar() const noexcept
{
	return m_far;
}

void Camera::SetFar(float farClip) noexcept
{
	m_far = farClip;
	CreateProjectionMatrix();
	CreateOrthographicMatrix();
}

const Math::Matrix& Camera::GetViewMatrix() const noexcept
{
	return GetTransform().GetWorldMatrix().Inverse();
}

const Math::Matrix& Camera::GetProjectionMatrix() const noexcept
{
	return m_projection;
}

const Math::Matrix& Camera::GetOrthographicMatrix() const noexcept
{
	return m_orthographic;
}

void Camera::CreateProjectionMatrix() noexcept
{
	if (IsInRenderingThread())
	{
		m_projection = Math::Matrix::CreatePerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
	}
	else
	{
		RegisterRenderCommand([this] {
			m_projection = Math::Matrix::CreatePerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
		});
	}
}

void Camera::CreateOrthographicMatrix() noexcept
{
	if (IsInRenderingThread())
	{
		m_orthographic = Math::Matrix::CreateOrthographicLH(m_width, m_height, m_near, m_far);
	}
	else
	{
		RegisterRenderCommand([this] {
			m_orthographic = Math::Matrix::CreateOrthographicLH(m_width, m_height, m_near, m_far);
		});
	}
}

void Camera::RegisterToRenderer() noexcept
{
	if (m_isRegister)
	{
		return;
	}

	m_isRegister = true;

	if (IsInRenderingThread())
	{
		m_renderer->AddCamera(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->AddCamera(this); });
	}
}

void Camera::UnRegisterFromRenderer() noexcept
{
	if (!m_isRegister)
	{
		return;
	}

	m_isRegister = false;

	if (IsInRenderingThread())
	{
		m_renderer->RemoveCamera(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->RemoveCamera(this); });
	}
}