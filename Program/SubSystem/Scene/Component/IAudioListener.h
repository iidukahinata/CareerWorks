/**
* @file    IAudioListener.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "IComponent.h"

class IAudioListener : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudioListener, IComponent)
public:

	virtual void SetVelocity(const Math::Vector3& velocity)
	{
		ASSERT(0);
	}

	virtual const Math::Vector3& GetVelocity() const
	{
		ASSERT(0); return {};
	}
};