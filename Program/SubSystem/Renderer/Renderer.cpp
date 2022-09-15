/**
* @file	   Renderer.h
* @brief
*
* @date	   2022/09/09 2022年度初版
*/


#include "Renderer.h"
#include "LightMap.h"
#include "SubSystem/Window/Window.h"
#include "GraphicsAPI/D3D12/D3D12GrahicsDevice.h"

bool Renderer::Initialize()
{
	const auto handle = Window::Get().GetHandle();
	const auto width = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	// デバイス初期化
	D3D12GrahicsDevice::Get().Init(handle, width, height, Window::Get().IsFullscreen());

	return true;
}

void Renderer::AddLight(Light* light) noexcept
{
	m_lightMap->AddLight(light);
}

void Renderer::RemoveLight(Light* light) noexcept
{
	m_lightMap->RemoveLight(light);
}

void Renderer::AddCamera(Camera* camera) noexcept
{
	if (m_cameras.contains(camera)) 
	{
		return;
	}

	if (!m_mainCamera) 
	{
		m_mainCamera = camera;
	}

	m_cameras.insert(camera);
}

void Renderer::RemoveCamera(Camera* camera) noexcept
{
	if (!m_cameras.contains(camera)) 
	{
		return;
	}

	if (m_mainCamera == camera)
	{
		m_mainCamera = nullptr;
	}

	m_cameras.erase(camera);
}

Camera* Renderer::GetMainCamera() const noexcept
{
	return m_mainCamera;
}

void Renderer::AddRenderObject(RenderObject* rederObject) noexcept
{
	if (m_renderObjects.contains(rederObject)) 
	{
		return;
	}

	m_renderObjects.insert(rederObject);
}

void Renderer::RemoveRenderObject(RenderObject* rederObject) noexcept
{
	if (!m_renderObjects.contains(rederObject)) 
	{
		return;
	}

	m_renderObjects.erase(rederObject);
}

void Renderer::RegisterPostProcess(PostProcessEffect* postProcess) noexcept
{
	m_postProcessEffect = postProcess;
}

void Renderer::OnRegisterPostProcess(PostProcessEffect* postProcess) noexcept
{
	if (m_postProcessEffect == postProcess)
	{
		m_postProcessEffect = nullptr;
	}
}