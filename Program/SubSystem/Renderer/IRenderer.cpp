/**
* @file	   IRenderer.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "IRenderer.h"
#include "ILightMap.h"
#include "SubSystem/Window/Window.h"
#include "GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Factory/ComponentFactory.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/Light.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Scene/Component/Components/PostProcessEffect.h"

bool IRenderer::Initialize()
{
	const auto handle = Window::Get().GetHandle();
	const auto width  = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	// デバイス初期化
	D3D12GraphicsDevice::Get().Init(handle, width, height, Window::Get().IsFullscreen());

	// register component
	ComponentFactory::Register<ICamera, Camera>();
	ComponentFactory::Register<ILight, Light>();
	ComponentFactory::Register<IMeshRender, MeshRender>();
	ComponentFactory::Register<IModelRender, ModelRender>();
	ComponentFactory::Register<IPostProcessEffect, PostProcessEffect>();

	return true;
}

void IRenderer::Shutdown()
{
	// unregister component
	ComponentFactory::UnRegister<ICamera>();
	ComponentFactory::UnRegister<ILight>();
	ComponentFactory::UnRegister<IMeshRender>();
	ComponentFactory::UnRegister<IModelRender>();
	ComponentFactory::UnRegister<IPostProcessEffect>();
}

void IRenderer::AddLight(ILight* light) noexcept
{
	m_lightMap->AddLight(light);
}

void IRenderer::RemoveLight(ILight* light) noexcept
{
	m_lightMap->RemoveLight(light);
}

void IRenderer::AddCamera(ICamera* camera) noexcept
{
	m_cameras.emplace_back(camera);

	if (!m_mainCamera) 
	{
		m_mainCamera = camera;
	}
}

void IRenderer::RemoveCamera(ICamera* camera) noexcept
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

ICamera* IRenderer::GetMainCamera() const noexcept
{
	return m_mainCamera;
}

void IRenderer::AddRenderObject(IRenderObject* rederObject) noexcept
{
	m_renderObjects.emplace_back(rederObject);
}

void IRenderer::RemoveRenderObject(IRenderObject* rederObject) noexcept
{
	std::erase(m_renderObjects, rederObject);
}

void IRenderer::RegisterPostProcess(IPostProcessEffect* postProcess) noexcept
{
	m_postProcessEffect = postProcess;
}

void IRenderer::UnRegisterPostProcess(IPostProcessEffect* postProcess) noexcept
{
	if (m_postProcessEffect == postProcess)
	{
		m_postProcessEffect = nullptr;
	}
}

IPostProcessEffect* IRenderer::GetPostProcess() noexcept
{
	return m_postProcessEffect;
}

bool IRenderer::HasPostProcessSetting() noexcept
{
	return !!m_postProcessEffect && !m_postProcessEffect->GetAllPostEffect().empty();
}