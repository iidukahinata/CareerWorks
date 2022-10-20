/**
 * @file	ISubsystem.cpp
 * @brief
 *
 * @date	2022/07/06 2022”N“x‰”Å
 */


#include "ISubsystem.h"

Context* ISubsystem::GetContext() const noexcept
{
	return g_context;
}