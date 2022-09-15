/**
* @file    Camera.cpp
* @brief
*
* @date	   2022/09/04 2022”N“x‰”Å
*/


#include "Camera.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Renderer/Renderer.h"

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

void Camera::Deserialization(FileStream* file)
{
	IComponent::Deserialization(file);

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

void Camera::Initialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();

	RegisterToRendererSystem();

	// init data
	m_fov = Math::ToRadian(45.0f);
	m_width = Window::Get().GetWindowWidth();
	m_height = Window::Get().GetWindowHeight();
	m_aspect = m_width / m_height;
	m_near = 0.1f;
	m_far = 1000.0f;

	// create matrix
	CreateProjectionMatrix();
	CreateOrthographicMatrix();

	GetTransform().SetPosition(Math::Vector3(0.f, 3.f, -8.f));
	GetTransform().LockAt(Math::Vector3::Zero);
}

void Camera::Remove()
{
	UnRegisterFromRedererSystem();
}

void Camera::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);
	if (active)
	{
		RegisterToRendererSystem();
	}
	else
	{
		UnRegisterFromRedererSystem();
	}
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
	m_near = nearClip;
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

const Math::Matrix& Camera::GetViewMatrix()
{
	return GetTransform().GetWorldMatrix().Inverse();
}

const Math::Matrix& Camera::GetProjectionMatrix()
{
	return m_projection;
}

const Math::Matrix& Camera::GetOrthographicMatrix()
{
	return m_orthographic;
}

void Camera::CreateProjectionMatrix()
{
	m_projection = Math::Matrix::CreatePerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
}

void Camera::CreateOrthographicMatrix()
{
	m_orthographic = Math::Matrix::CreateOrthographicLH(m_width, m_height, m_near, m_far);
}

void Camera::RegisterToRendererSystem()
{
	ASSERT(m_renderer);
	m_renderer->AddCamera(this);
}

void Camera::UnRegisterFromRedererSystem()
{
	ASSERT(m_renderer);
	m_renderer->RemoveCamera(this);
}