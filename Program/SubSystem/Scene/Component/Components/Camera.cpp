/**
* @file    Camera.cpp
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
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

void Camera::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();

	// init data
	m_width  = Window::Get().GetWindowWidth();
	m_height = Window::Get().GetWindowHeight();
	m_aspect = m_width / m_height;
	m_fov	 = Math::ToRadian(45.0f);
	m_near	 = 0.1f;
	m_far	 = 1000.0f;

	// create matrix
	CreateProjectionMatrix();
	CreateOrthographicMatrix();
}

void Camera::OnRegister()
{
	IComponent::OnRegister();

	RegisterToRendererSystem();
}

void Camera::OnUnRegister()
{
	IComponent::OnUnRegister();

	UnRegisterFromRedererSystem();
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
	m_projection = Math::Matrix::CreatePerspectiveFovLH(m_fov, m_aspect, m_near, m_far);
}

void Camera::CreateOrthographicMatrix() noexcept
{
	m_orthographic = Math::Matrix::CreateOrthographicLH(m_width, m_height, m_near, m_far);
}

void Camera::RegisterToRendererSystem() noexcept
{
	ASSERT(m_renderer);
	m_renderer->AddCamera(this);
}

void Camera::UnRegisterFromRedererSystem() noexcept
{
	ASSERT(m_renderer);
	m_renderer->RemoveCamera(this);
}