/**
* @file	   PostProcessEffectDetails.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../ComponentDetails.h"
#include "SubSystem/Renderer/PostEffect/IPostEffect.h"

class IPostProcessEffect;

class PostProcessEffectDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	PostProcessEffectDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	/** �g�p�� PostEffect �̕\�� */
	void ShowAllPostEffect() noexcept;

	/** �|�X�g�G�t�F�N�g�ǉ��p window �̕\�����s���B*/
	void ShowAddPostEffectWindow() noexcept;

	template<class T>
	void RegisterPostEffect();

private:

	void ShowBloom(IPostEffect* postEffect) noexcept;

private:

	// * AddPostEffect �ɕ\������|�X�g�G�t�F�N�g�ꗗ
	Vector<UniquePtr<IPostEffect>> m_postEffects;

	// * AddPostEffect �p�t�B���^�[
	ImGuiTextFilter m_filter;

	IPostProcessEffect* m_postProcessEffect = nullptr;
};

template<class T>
FORCEINLINE void PostProcessEffectDetails::RegisterPostEffect()
{
	m_postEffects.emplace_back(std::make_unique<T>());
}