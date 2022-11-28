/**
* @file	   PostProcessEffectDetails.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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

	/** 使用中 PostEffect の表示 */
	void ShowAllPostEffect() noexcept;

	/** ポストエフェクト追加用 window の表示を行う。*/
	void ShowAddPostEffectWindow() noexcept;

	template<class T>
	void RegisterPostEffect();

private:

	void ShowBloom(IPostEffect* postEffect) noexcept;

private:

	// * AddPostEffect に表示するポストエフェクト一覧
	Vector<UniquePtr<IPostEffect>> m_postEffects;

	// * AddPostEffect 用フィルター
	ImGuiTextFilter m_filter;

	IPostProcessEffect* m_postProcessEffect = nullptr;
};

template<class T>
FORCEINLINE void PostProcessEffectDetails::RegisterPostEffect()
{
	m_postEffects.emplace_back(std::make_unique<T>());
}