/**
* @file    PostEffectFactory.h
* @brief
*
* @date	   2022/10/29 2022�N�x����
*/
#pragma once


#include "../PostEffect/IPostEffect.h"

/**
* �|�X�g�G�t�F�N�g�̐������V�X�e�����番�������邽�߂ɍ쐬�B
* �V�����|�X�g�G�t�F�N�g���쐬�����ꍇ�A���L�֐����ɒǉ����Ă����B
*/
struct PostEffectFactory
{
	static UniquePtr<IPostEffect> Create(PostProcessEffect* postProcessEffect, StringView name) noexcept;
};