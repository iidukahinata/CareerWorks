/**
* @file    PostProcessEffect.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
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

	/** �w�薼�G�t�F�N�g�ǉ��B�w�薼�����݂��Ȃ����̏ꍇ�Anull ��Ԃ��B*/
	IPostEffect* AddPostEffect(StringView name) override;

	/** �ێ�����G�t�F�N�g���������邽�߁A���̃A�h���X�������Ƃ���B*/
	void RemovePostEffect(IPostEffect* postEffect) override;

	/** �w�薼�G�t�F�N�g��ێ�����ꍇ�A���̃A�h���X��Ԃ��B*/
	IPostEffect* FindPostEffect(StringView name) const override;

private:

	void AddPostEffect(IPostEffect* postEffect) noexcept;

public:

	void GetAllPostEffect(Vector<IPostEffect*>& postEffects) const noexcept;
	const Map<uint32_t, UniquePtr<IPostEffect>>& GetAllPostEffect() override;

	bool HasPostEffect() const noexcept;

	/** �A�N�Z�X */
	IRenderer* GetRenderer() const noexcept;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

private:

	IRenderer* m_renderer = nullptr;

	bool m_isRegister = false;

	// * -> �n�b�V���l : �e�G�t�F�N�g�I�u�W�F�N�g
	Map<uint32_t, UniquePtr<IPostEffect>> m_postEffects;

	RenderCommandFance m_renderCommandFance;
};