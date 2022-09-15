/**
 * @file	ISubsystem.cpp
 * @brief
 *
 * @date	2022/07/06 2022îNìxèâî≈
 */


#include "ISubsystem.h"
#include "Context.h"

extern Context* g_context;

Context* ISubsystem::GetContext() const noexcept
{
	return g_context;
}