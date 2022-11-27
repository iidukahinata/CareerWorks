/**
* @file    PostEffectFactory.cpp
* @brief
*
* @date	   2022/10/29 2022îNìxèâî≈
*/


#include "PostEffectFactory.h"
#include "SubSystem/Renderer/PostEffect/Blur.h"
#include "SubSystem/Renderer/PostEffect/Bloom.h"
#include "SubSystem/Renderer/PostEffect/DepthOfField.h"

#define CASE_CREATE_POSTEFFECT(CLASS, POSTEFFECT) case GET_HASH(CLASS): POSTEFFECT = std::make_unique<CLASS>(); break;

UniquePtr<IPostEffect> PostEffectFactory::Create(PostProcessEffect* postProcessEffect, StringView name) noexcept
{
    UniquePtr<IPostEffect> postEffect;
    const PostEffectType type(name);

    switch (type.Hash)
    {
    CASE_CREATE_POSTEFFECT(Bloom       , postEffect);
    CASE_CREATE_POSTEFFECT(DepthOfField, postEffect);
    default: break;
    }

    if (postEffect)
    {
        postEffect->m_postProcessEffect = postProcessEffect;
    }

    return postEffect;
}