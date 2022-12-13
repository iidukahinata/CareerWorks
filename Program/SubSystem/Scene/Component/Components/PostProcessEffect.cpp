/**
* @file    PostProcessEffect.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/

#include "PostProcessEffect.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Renderer/IRenderer.h"
#include "SubSystem/Renderer/PostEffect/IPostEffect.h"
#include "SubSystem/Renderer/Factory/PostEffectFactory.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12RenderTexture.h"

void PostProcessEffect::Serialized(FileStream* file) const
{
	size_t numPostEffect = m_postEffects.size();
	file->Write(numPostEffect);

	for (const auto& postEffectInfo : m_postEffects)
	{
		const auto& postEffect = postEffectInfo.second;
		auto postEffectName = postEffect->GetTypeData().Name;

		file->Write(postEffectName);
		postEffect->Serialized(file);
	}
}

void PostProcessEffect::Deserialized(FileStream* file)
{
	const auto width  = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	auto proj = Math::Matrix::CreateOrthographicLH(width, height, 0.1f, 100.0f);
	auto view = Math::Matrix(Math::Vector3(0, 0, 1), Math::Vector3::Zero, Math::Vector3::One);

	if (IsInRenderingThread())
	{
		GetRenderer()->GetTransformCBuffer()->Update(view.ToMatrixXM(), proj.ToMatrixXM());
	}
	else
	{
		RegisterRenderCommand([this, view, proj] {

			GetRenderer()->GetTransformCBuffer()->Update(view.ToMatrixXM(), proj.ToMatrixXM());

		});
	}

	size_t numPostEffect = 0;
	file->Read(&numPostEffect);

	for (int i = 0; i < numPostEffect; ++i)
	{
		String postEffectName;
		file->Read(&postEffectName);

		auto postEffect = AddPostEffect(postEffectName);
		postEffect->Deserialized(file);
	}
}

void PostProcessEffect::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<IRenderer>();
	ASSERT(m_renderer);
}

void PostProcessEffect::OnRegister()
{
	IComponent::OnRegister();

	if (GetActive())
	{
		RegisterToRenderer();
	}
}

void PostProcessEffect::OnUnRegister()
{
	IComponent::OnUnRegister();

	if (GetActive())
	{
		UnRegisterFromRenderer();
	}
}

void PostProcessEffect::OnRemove()
{
	UnRegisterFromRenderer();

	m_renderCommandFance.BegineFrame();

	IComponent::OnRemove();
}

void PostProcessEffect::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);

	if (active)
	{
		RegisterToRenderer();
	}
	else
	{
		UnRegisterFromRenderer();
	}
}

bool PostProcessEffect::Erasable()
{
	return m_renderCommandFance.IsSingle();
}

IPostEffect* PostProcessEffect::AddPostEffect(StringView name)
{
	IPostEffect* result = nullptr;

	if (auto postEffect = PostEffectFactory::Create(this, name))
	{
		result = postEffect.get();
		AddPostEffect(postEffect.release());
	}

	return result;
}

void PostProcessEffect::AddPostEffect(IPostEffect* postEffect) noexcept
{
	ASSERT(postEffect);

	const auto hash = postEffect->GetTypeData().Hash;

	if (!m_postEffects.contains(hash))
	{
		postEffect->Initialize();

		m_postEffects.emplace(hash, postEffect);
	}
	else
	{
		LOG_ERROR("既に同じHash値のエフェクトが存在しています。");
	}
}

void PostProcessEffect::RemovePostEffect(IPostEffect* postEffect)
{
	ASSERT(postEffect);

	const auto hash = postEffect->GetTypeData().Hash;
	if (m_postEffects.contains(hash))
	{
		m_postEffects.erase(hash);
	}
}

IPostEffect* PostProcessEffect::FindPostEffect(StringView name) const
{
	const auto type = PostEffectType(name);

	if (m_postEffects.contains(type.Hash))
	{
		return m_postEffects.at(type.Hash).get();
	}

	return nullptr;
}

void PostProcessEffect::GetAllPostEffect(Vector<IPostEffect*>& postEffects) const noexcept
{
	postEffects.resize(m_postEffects.size());

	for (const auto& postEffectInfo : m_postEffects)
	{
		postEffects.push_back(postEffectInfo.second.get());
	}
}

const Map<uint32_t, UniquePtr<IPostEffect>>& PostProcessEffect::GetAllPostEffect()
{
	return m_postEffects;
}

bool PostProcessEffect::HasPostEffect() const noexcept
{
	return !m_postEffects.empty();
}

IRenderer* PostProcessEffect::GetRenderer() const noexcept
{
	return m_renderer;
}

void PostProcessEffect::Render()
{
	IPostEffect* lastPostEffect = nullptr;
	for (const auto& postEffectInfo : m_postEffects)
	{
		const auto& postEffect = postEffectInfo.second;

		postEffect->Render();
		lastPostEffect = postEffect.get();
	}

	if (!m_postEffects.empty())
	{
		lastPostEffect->GetTexture().PSSet(0);
	}
}

void PostProcessEffect::RegisterToRenderer() noexcept
{
	if (m_isRegister)
	{
		return;
	}

	m_isRegister = true;

	if (IsInRenderingThread())
	{
		m_renderer->RegisterPostProcess(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->RegisterPostProcess(this); });
	}
}

void PostProcessEffect::UnRegisterFromRenderer() noexcept
{
	if (!m_isRegister)
	{
		return;
	}

	m_isRegister = false;

	if (IsInRenderingThread())
	{
		m_renderer->UnRegisterPostProcess(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->UnRegisterPostProcess(this); });
	}
}