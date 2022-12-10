/**
* @file    Camera.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "Camera.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Renderer/IRenderer.h"
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
	m_renderer = GetContext()->GetSubsystem<IRenderer>();
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
	UnRegisterFromRenderer();

	m_renderCommandFance.BegineFrame();

	IComponent::OnRemove();
}

bool Camera::Erasable()
{
	return m_renderCommandFance.IsSingle();
}

float Camera::GetWidth() const
{
	return m_width;
}

void Camera::SetWidth(float width)
{
	m_width = width;
	CreateOrthographicMatrix();
}

float Camera::GetHeight() const
{
	return m_height;
}

void Camera::SetHeight(float height)
{
	m_height = height;
	CreateOrthographicMatrix();
}

float Camera::GetAspect() const
{
	return m_aspect;
}

void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
	CreateProjectionMatrix();
}

float Camera::GetFov() const
{
	return m_fov;
}

void Camera::SetFov(float fov)
{
	m_fov = fov;
	CreateProjectionMatrix();
}

float Camera::GetNear() const
{
	return m_near;
}

void Camera::SetNear(float nearClip)
{
	m_near = max(0.01f, nearClip);
	CreateProjectionMatrix();
	CreateOrthographicMatrix();
}

float Camera::GetFar() const
{
	return m_far;
}

void Camera::SetFar(float farClip)
{
	m_far = farClip;
	CreateProjectionMatrix();
	CreateOrthographicMatrix();
}

const Math::Matrix& Camera::GetViewMatrix() const
{
	return GetTransform().GetWorldMatrix().Inverse();
}

const Math::Matrix& Camera::GetProjectionMatrix() const
{
	return m_projection;
}

const Math::Matrix& Camera::GetOrthographicMatrix() const
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