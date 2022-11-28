/**
* @file    ICollider.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "IComponent.h"

enum ShapeType
{
	Box,

	Sphere,

	Capsule,

	//Mesh,

	//ConvexMesh,
};

class ICollider : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ICollider, IComponent)
public:

	/** shape */
	virtual void SetShapeType(ShapeType type)
	{
		ASSERT(0);
	}
	virtual ShapeType GetShapeType() const
	{
		ASSERT(0); return {};
	}
	virtual void SetTrigger(bool trigger)
	{
		ASSERT(0);
	}
	virtual bool GetTrigger() const
	{
		ASSERT(0); return {};
	}

	/** scale */
	virtual void SetScale(const Math::Vector3& scale)
	{
		ASSERT(0);
	}
	virtual const Math::Vector3& GetScale() const
	{
		ASSERT(0); return {};
	}
	virtual float GetRadius() const
	{
		ASSERT(0); return {};
	}
	virtual float GetHight() const
	{
		ASSERT(0); return {};
	}

	/** offset */
	virtual void SetRestOffset(float restOffset)
	{
		ASSERT(0);
	}
	virtual float GetRestOffset() const
	{
		ASSERT(0); return {};
	}
	virtual void SetContactOffset(float contactOffset)
	{
		ASSERT(0);
	}
	virtual float GetContactOffset() const
	{
		ASSERT(0); return {};
	}

	/** material */
	virtual void SetStaticFriction(float friction)
	{
		ASSERT(0);
	}
	virtual float GetStaticFriction() const
	{
		ASSERT(0); return {};
	}
	virtual void SetDynamicFriction(float friction)
	{
		ASSERT(0);
	}
	virtual float GetDynamicFriction() const
	{
		ASSERT(0); return {};
	}
	virtual void SetRestitution(float restitution)
	{
		ASSERT(0);
	}
	virtual float GetRestitution() const
	{
		ASSERT(0); return {};
	}
};