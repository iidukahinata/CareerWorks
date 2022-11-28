/**
* @file    ICamera.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "IComponent.h"

class ICamera : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ICamera, IComponent)
public:

	/** width */
	virtual float GetWidth() const
	{
		ASSERT(0); return {};
	}
	virtual void SetWidth(float width)
	{
		ASSERT(0);
	}

	/** height */
	virtual float GetHeight() const
	{
		ASSERT(0); return {};
	}
	virtual void SetHeight(float height)
	{
		ASSERT(0);
	}

	/** aspect */
	virtual float GetAspect() const
	{
		ASSERT(0); return {};
	}
	virtual void SetAspect(float aspect)
	{
		ASSERT(0);
	}

	/** fav */
	virtual float GetFov() const
	{
		ASSERT(0); return {};
	}
	virtual void SetFov(float fov)
	{
		ASSERT(0);
	}

	/** near */
	virtual float GetNear() const
	{
		ASSERT(0); return {};
	}
	virtual void SetNear(float nearClip)
	{
		ASSERT(0);
	}

	/** far */
	virtual float GetFar() const
	{
		ASSERT(0); return {};
	}
	virtual void SetFar(float farClip)
	{
		ASSERT(0);
	}

	/** matrix objects */
	virtual const Math::Matrix& GetViewMatrix() const
	{
		ASSERT(0); return {};
	}
	virtual const Math::Matrix& GetProjectionMatrix() const
	{
		ASSERT(0); return {};
	}
	virtual const Math::Matrix& GetOrthographicMatrix() const
	{
		ASSERT(0); return {};
	}
};