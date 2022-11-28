/**
* @file    IPostProcessEffect.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
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

	/** �w�薼�G�t�F�N�g�ǉ��B�w�薼�����݂��Ȃ����̏ꍇ�Anull ��Ԃ��B*/
	virtual IPostEffect* AddPostEffect(StringView name)
	{
		ASSERT(0); return {};
	}

	/** �ێ�����G�t�F�N�g���������邽�߁A���̃A�h���X�������Ƃ���B*/
	virtual void RemovePostEffect(IPostEffect* postEffect)
	{
		ASSERT(0);
	}

	/** �w�薼�G�t�F�N�g��ێ�����ꍇ�A���̃A�h���X��Ԃ��B*/
	virtual IPostEffect* FindPostEffect(StringView name) const
	{
		ASSERT(0); return {};
	}

	/** �^������R���|�[�l���g�������B*/
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