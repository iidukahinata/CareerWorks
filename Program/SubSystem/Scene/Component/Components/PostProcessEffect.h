/**
* @file    PostProcessEffect.h
* @brief
*
* @date	   2022/10/29 2022年度初版
*/
#pragma once


#include "../IComponent.h"
#include "SubSystem/Renderer/PostEffect/PostEffect.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"

class Renderer;

class PostProcessEffect : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(PostProcessEffect)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	bool Erasable() override;

public:

	void Render() noexcept;

public:

	/** 指定名エフェクト追加。指定名が存在しない等の場合、null を返す。*/
	PostEffect* AddPostEffect(StringView name) noexcept;
	void AddPostEffect(PostEffect* postEffect) noexcept;

	/** 保持するエフェクトを消去するため、そのアドレスを引数とする。*/
	void RemovePostEffect(PostEffect* postEffect) noexcept;

	/** 型名からコンポーネントを検索。*/
	template<class T>
	T* GetPostEffect();
	void GetAllPostEffect(Vector<PostEffect*>& postEffects) const noexcept;
	const Map<uint32_t, UniquePtr<PostEffect>>& GetAllPostEffect() noexcept;
	bool HasPostEffect() const noexcept;

	/** 指定名エフェクトを保持する場合、そのアドレスを返す。*/
	PostEffect* FindPostEffect(StringView name) const noexcept;

	/** アクセス */
	Renderer* GetRenderer() const noexcept;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

private:

	Renderer* m_renderer = nullptr;

	// * -> ハッシュ値 : 各エフェクトオブジェクト
	Map<uint32_t, UniquePtr<PostEffect>> m_postEffects;

	RenderCommandFance m_renderCommandFance;
};

template<class T>
FORCEINLINE T* PostProcessEffect::GetPostEffect()
{
	return dynamic_cast<T*>(FindPostEffect(T::TypeData.Name));
}