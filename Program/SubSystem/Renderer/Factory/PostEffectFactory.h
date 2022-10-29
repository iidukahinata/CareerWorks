/**
* @file    PostEffectFactory.h
* @brief
*
* @date	   2022/10/29 2022年度初版
*/
#pragma once


#include "../PostEffect/PostEffect.h"

/**
* ポストエフェクトの生成をシステムから分離させるために作成。
* 新しいポストエフェクトを作成した場合、下記関数内に追加していく。
*/
struct PostEffectFactory
{
	static UniquePtr<PostEffect> Create(PostProcessEffect* postProcessEffect, StringView name) noexcept;
};