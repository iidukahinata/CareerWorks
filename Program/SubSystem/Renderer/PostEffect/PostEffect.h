/**
* @file    PostEffect.h
* @brief
*
* @date	   2022/10/29 2022îNìxèâî≈
*/
#pragma once


class PostProcessEffect;

using PostEffectType = ClassTypeData;

class PostEffect
{
	IN_DEVELOPMENT()
	SUPER_CLASS(PostEffect)

	friend class PostEffectFactory;
public:

	virtual ~PostEffect() = default;

	virtual void Serialized(FileStream* file) const = 0;
	virtual void Deserialized(FileStream* file) = 0;

	virtual void Initialize() = 0;

	virtual void Render() = 0;

private:

	PostProcessEffect* m_postProcessEffect = nullptr;
};