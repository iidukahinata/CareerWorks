/**
* @file    PostEffect.cpp
* @brief
*
* @date	   2022/10/29 2022”N“x‰”Å
*/


#include "IPostEffect.h"
#include "../IRenderer.h"
#include "Subsystem/Scene/Component/Components/PostProcessEffect.h"

IRenderer* IPostEffect::GetRenderer() const noexcept
{
	ASSERT(m_postProcessEffect);
	return m_postProcessEffect->GetRenderer();
}