/**
* @file    IPostProcessEffect.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "IComponent.h"
#include "SubSystem/Renderer/PostEffect/IPostEffect.h"

class IPostProcessEffect : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IPostProcessEffect, IComponent)
public:

	virtual void Render()
	{
		ASSERT(0);
	}

public:

	/** 指定名エフェクト追加。指定名が存在しない等の場合、null を返す。*/
	virtual IPostEffect* AddPostEffect(StringView name)
	{
		ASSERT(0); return {};
	}

	/** 保持するエフェクトを消去するため、そのアドレスを引数とする。*/
	virtual void RemovePostEffect(IPostEffect* postEffect)
	{
		ASSERT(0);
	}

	/** 指定名エフェクトを保持する場合、そのアドレスを返す。*/
	virtual IPostEffect* FindPostEffect(StringView name) const
	{
		ASSERT(0); return {};
	}

	/** 型名からコンポーネントを検索。*/
	template<class T>
	T* GetPostEffect();

public:

	virtual const Map<uint32_t, UniquePtr<IPostEffect>>& GetAllPostEffect()
	{
		ASSERT(0); return {};
	}
};

template<class T>
FORCEINLINE T* IPostProcessEffect::GetPostEffect()
{
	return dynamic_cast<T*>(FindPostEffect(T::TypeData.Name));
}