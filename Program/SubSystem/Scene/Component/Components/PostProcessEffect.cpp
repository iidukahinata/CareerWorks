/**
* @file    PostProcessEffect.cpp
* @brief
*
* @date	   2022/10/29 2022�N�x����
*/

#include "PostProcessEffect.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Renderer/PostEffect/PostEffect.h"
#include "SubSystem/Renderer/Factory/PostEffectFactory.h"

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
	m_renderer = GetContext()->GetSubsystem<Renderer>();
}

void PostProcessEffect::OnRegister()
{
	IComponent::OnRegister();

	RegisterToRenderer();
}

void PostProcessEffect::OnUnRegister()
{
	IComponent::OnUnRegister();

	UnRegisterFromRenderer();
}

PostEffect* PostProcessEffect::AddPostEffect(StringView name) noexcept
{
	PostEffect* result = nullptr;

	if (auto postEffect = PostEffectFactory::Create(this, name))
	{
		result = postEffect.get();
		AddPostEffect(postEffect.release());
	}

	return result;
}

void PostProcessEffect::AddPostEffect(PostEffect* postEffect) noexcept
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
		LOG_ERROR("���ɓ���Hash�l�̃G�t�F�N�g�����݂��Ă��܂��B");
	}
}

void PostProcessEffect::RemovePostEffect(PostEffect* postEffect) noexcept
{
	ASSERT(postEffect);

	const auto hash = postEffect->GetTypeData().Hash;
	if (m_postEffects.contains(hash))
	{
		m_postEffects.erase(hash);
	}
}

void PostProcessEffect::GetAllPostEffect(Vector<PostEffect*>& postEffects) const noexcept
{
	postEffects.resize(m_postEffects.size());

	for (const auto& postEffectInfo : m_postEffects)
	{
		postEffects.push_back(postEffectInfo.second.get());
	}
}

const Map<uint32_t, UniquePtr<PostEffect>>& PostProcessEffect::GetAllPostEffect() noexcept
{
	return m_postEffects;
}

PostEffect* PostProcessEffect::FindPostEffect(StringView name) const noexcept
{
	const auto type = PostEffectType(name);

	if (m_postEffects.contains(type.Hash))
	{
		return m_postEffects.at(type.Hash).get();
	}

	return nullptr;
}

void PostProcessEffect::Render() noexcept
{
	for (const auto& postEffectInfo : m_postEffects)
	{
		const auto& postEffect = postEffectInfo.second;

		postEffect->Render();
	}
}

void PostProcessEffect::RegisterToRenderer() noexcept
{
	ASSERT(m_renderer);
	m_renderer->RegisterPostProcess(this);
}

void PostProcessEffect::UnRegisterFromRenderer() noexcept
{
	ASSERT(m_renderer);
	m_renderer->UnRegisterPostProcess(this);
}