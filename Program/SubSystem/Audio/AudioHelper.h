/**
* @file		AudioHelper.h
* @brief
*
* @date		2022/08/02 2022îNìxèâî≈
*/
#pragma once


#include <fmod.hpp>
#include <fmod_errors.h>

#define AUDIO_EORROR_CHECK(data)								\
if (data != FMOD_OK)											\
{																\
	LOG_ERROR(String("FMOD ERROR : ") + FMOD_ErrorString(data));\
}

constexpr FMOD_VECTOR ToFMODVector(const Math::Vector3 & v)
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}