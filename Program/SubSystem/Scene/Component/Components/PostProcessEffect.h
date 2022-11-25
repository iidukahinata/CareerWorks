/**
* @file    PostProcessEffect.h
* @brief
*
* @date	   2022/10/29 2022�N�x����
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

	/** �w�薼�G�t�F�N�g�ǉ��B�w�薼�����݂��Ȃ����̏ꍇ�Anull ��Ԃ��B*/
	PostEffect* AddPostEffect(StringView name) noexcept;
	void AddPostEffect(PostEffect* postEffect) noexcept;

	/** �ێ�����G�t�F�N�g���������邽�߁A���̃A�h���X�������Ƃ���B*/
	void RemovePostEffect(PostEffect* postEffect) noexcept;

	/** �^������R���|�[�l���g�������B*/
	template<class T>
	T* GetPostEffect();
	void GetAllPostEffect(Vector<PostEffect*>& postEffects) const noexcept;
	const Map<uint32_t, UniquePtr<PostEffect>>& GetAllPostEffect() noexcept;
	bool HasPostEffect() const noexcept;

	/** �w�薼�G�t�F�N�g��ێ�����ꍇ�A���̃A�h���X��Ԃ��B*/
	PostEffect* FindPostEffect(StringView name) const noexcept;

	/** �A�N�Z�X */
	Renderer* GetRenderer() const noexcept;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

private:

	Renderer* m_renderer = nullptr;

	// * -> �n�b�V���l : �e�G�t�F�N�g�I�u�W�F�N�g
	Map<uint32_t, UniquePtr<PostEffect>> m_postEffects;

	RenderCommandFance m_renderCommandFance;
};

template<class T>
FORCEINLINE T* PostProcessEffect::GetPostEffect()
{
	return dynamic_cast<T*>(FindPostEffect(T::TypeData.Name));
}