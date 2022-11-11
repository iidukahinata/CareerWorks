/**
* @file    PostEffect.cpp
* @brief
*
* @date	   2022/10/29 2022”N“x‰”Å
*/


#include "PostEffect.h"
#include "../Renderer.h"
#include "Subsystem/Scene/Component/Components/PostProcessEffect.h"

Renderer* PostEffect::GetRenderer() const noexcept
{
	ASSERT(m_postProcessEffect);
	return m_postProcessEffect->GetRenderer();
}