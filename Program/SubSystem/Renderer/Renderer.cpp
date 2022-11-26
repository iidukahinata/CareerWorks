/**
* @file	   Renderer.h
* @brief
*
* @date	   2022/09/09 2022年度初版
*/


#include "Renderer.h"
#include "LightMap.h"
#include "SubSystem/Window/Window.h"
#include "GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/ComponentCollection.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/Light.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Scene/Component/Components/PostProcessEffect.h"

bool Renderer::Initialize()
{
	const auto handle = Window::Get().GetHandle();
	const auto width  = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	// デバイス初期化
	D3D12GraphicsDevice::Get().Init(handle, width, height, Window::Get().IsFullscreen());

	// register component
	ComponentCollection::Register<Camera, Camera>();
	ComponentCollection::Register<Light, Light>();
	ComponentCollection::Register<MeshRender, MeshRender>();
	ComponentCollection::Register<ModelRender, ModelRender>();
	ComponentCollection::Register<PostProcessEffect, PostProcessEffect>();

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
	m_cameras.emplace_back(camera);

	if (!m_mainCamera) 
	{
		m_mainCamera = camera;
	}
}

void Renderer::RemoveCamera(Camera* camera) noexcept
{
	std::erase(m_cameras, camera);

	if (m_mainCamera == camera)
	{
		if (m_cameras.empty())
		{
			m_mainCamera = nullptr;
		}
		else
		{
			m_mainCamera = m_cameras.front();
		}
	}
}

Camera* Renderer::GetMainCamera() const noexcept
{
	return m_mainCamera;
}

void Renderer::AddRenderObject(RenderObject* rederObject) noexcept
{
	m_renderObjects.emplace_back(rederObject);
}

void Renderer::RemoveRenderObject(RenderObject* rederObject) noexcept
{
	std::erase(m_renderObjects, rederObject);
}

void Renderer::RegisterPostProcess(PostProcessEffect* postProcess) noexcept
{
	m_postProcessEffect = postProcess;
}

void Renderer::UnRegisterPostProcess(PostProcessEffect* postProcess) noexcept
{
	if (m_postProcessEffect == postProcess)
	{
		m_postProcessEffect = nullptr;
	}
}

PostProcessEffect* Renderer::GetPostProcess() noexcept
{
	return m_postProcessEffect;
}

bool Renderer::HasPostProcessSetting() noexcept
{
	return !!m_postProcessEffect && m_postProcessEffect->GetActive() && m_postProcessEffect->HasPostEffect();
}