/**
* @file    PostProcessEffect.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IPostProcessEffect.h"
#include "SubSystem/Renderer/PostEffect/IPostEffect.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"

class IRenderer;

class PostProcessEffect : public IPostProcessEffect
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(PostProcessEffect, IPostProcessEffect)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	void SetActive(bool active) override;
	bool Erasable() override;

public:

	void Render() override;

public:

	/** 指定名エフェクト追加。指定名が存在しない等の場合、null を返す。*/
	IPostEffect* AddPostEffect(StringView name) override;

	/** 保持するエフェクトを消去するため、そのアドレスを引数とする。*/
	void RemovePostEffect(IPostEffect* postEffect) override;

	/** 指定名エフェクトを保持する場合、そのアドレスを返す。*/
	IPostEffect* FindPostEffect(StringView name) const override;

private:

	void AddPostEffect(IPostEffect* postEffect) noexcept;

public:

	void GetAllPostEffect(Vector<IPostEffect*>& postEffects) const noexcept;
	const Map<uint32_t, UniquePtr<IPostEffect>>& GetAllPostEffect() override;

	bool HasPostEffect() const noexcept;

	/** アクセス */
	IRenderer* GetRenderer() const noexcept;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

private:

	IRenderer* m_renderer = nullptr;

	bool m_isRegister = false;

	// * -> ハッシュ値 : 各エフェクトオブジェクト
	Map<uint32_t, UniquePtr<IPostEffect>> m_postEffects;

	RenderCommandFance m_renderCommandFance;
};