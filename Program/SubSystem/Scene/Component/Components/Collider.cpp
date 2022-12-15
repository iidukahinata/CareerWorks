/**
* @file    Collider.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "Collider.h"
#include "../IRigidBody.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Physics/PhysX/PhysX.h"

void Collider::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	file->Write(m_shapeType);
	file->Write(m_scale);
	file->Write(m_isTrigger);
	file->Write(m_restOffset);
	file->Write(m_contactOffset);
	file->Write(m_staticFriction);
	file->Write(m_dynamicFriction);
	file->Write(m_restitution);
}

void Collider::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	file->Read(&m_shapeType);
	file->Read(&m_scale);
	file->Read(&m_isTrigger);
	file->Read(&m_restOffset);
	file->Read(&m_contactOffset);
	file->Read(&m_staticFriction);
	file->Read(&m_dynamicFriction);
	file->Read(&m_restitution);

	// init data
	SetShapeType(m_shapeType);
	SetStaticFriction(m_staticFriction);
	SetDynamicFriction(m_dynamicFriction);
	SetRestitution(m_restitution);
}

void Collider::OnInitialize()
{
	m_physics = dynamic_cast<PhysX*>(GetContext()->GetSubsystem<IPhysics>());
	ASSERT(m_physics);

	// Create Material
	{
		auto physics = m_physics->GetDevice();
		m_material = physics->createMaterial(0, 0, 0);
		ASSERT(m_material);
	}

#if IS_EDITOR
	if (!GetWorld()->IsGameMode())
	{
		SetShapeType(m_shapeType);
	}
#endif // IS_EDITOR
}

void Collider::OnStart()
{
	IComponent::OnStart();

	if (GetActive())
	{
		AttachToRegidBody();
	}
}

void Collider::OnStop()
{
	IComponent::OnStop();

	if (GetActive())
	{
		DetachFromRegidBody();
	}
}

void Collider::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);

	if (active)
	{
		AttachToRegidBody();
	}
	else
	{
		DetachFromRegidBody();
	}
}

void Collider::SetShapeType(ShapeType type)
{
	m_shapeType = type;

	m_shape = CreateShape();

	physx::PxFilterData filter;
	filter.word0 = 1;
	filter.word1 = 1;
	m_shape->setSimulationFilterData(filter);

	// set prams
	SetTrigger(m_isTrigger);
	SetRestOffset(m_restOffset);
	SetContactOffset(m_contactOffset);

	m_shape->userData = this;
}

ShapeType Collider::GetShapeType() const
{
	return m_shapeType;
}

void Collider::SetTrigger(bool trigger)
{
	m_isTrigger = trigger;

	if (trigger) 
	{
		m_shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		m_shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE	  , true);
	}
	else
	{
		m_shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		m_shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	}
}

bool Collider::GetTrigger() const
{
	return m_isTrigger;
}

void Collider::SetScale(const Math::Vector3& scale)
{
	m_scale = scale;
	UpdateGeometry();
}

const Math::Vector3& Collider::GetScale() const
{
	return m_scale;
}

float Collider::GetRadius() const
{
	return m_scale.x;
}

float Collider::GetHight() const
{
	return m_scale.y;
}

void Collider::SetRestOffset(float restOffset)
{
	m_restOffset = restOffset;
	m_shape->setRestOffset(restOffset);
}

float Collider::GetRestOffset() const
{
	return m_restOffset;
}

void Collider::SetContactOffset(float contactOffset)
{
	m_contactOffset = contactOffset;
	m_shape->setContactOffset(contactOffset);
}

float Collider::GetContactOffset() const
{
	return m_contactOffset;
}

void Collider::SetStaticFriction(float friction)
{
	m_staticFriction = friction;
	m_material->setStaticFriction(friction);
}

float Collider::GetStaticFriction() const
{
	return m_staticFriction;
}

void Collider::SetDynamicFriction(float friction)
{
	m_dynamicFriction = friction;
	m_material->setDynamicFriction(friction);
}

float Collider::GetDynamicFriction() const
{
	return m_dynamicFriction;
}

void Collider::SetRestitution(float restitution)
{
	m_restitution = restitution;
	m_material->setRestitution(restitution);
}

float Collider::GetRestitution() const
{
	return m_restitution;
}

physx::PxShape* Collider::GetShape() noexcept
{
	if (!m_shape || m_shape->isReleasable())
	{
		SetShapeType(m_shapeType);
	}

	return m_shape;
}

physx::PxShape* Collider::CreateShape() const noexcept
{
	auto physics = m_physics->GetDevice();

	switch (m_shapeType)
	{
	case Box:	  return physics->createShape(physx::PxBoxGeometry(m_scale.x, m_scale.y, m_scale.z), *m_material, true, physx::PxShapeFlag::eSIMULATION_SHAPE);
	case Sphere:  return physics->createShape(physx::PxSphereGeometry(m_scale.x)				   , *m_material, true, physx::PxShapeFlag::eSIMULATION_SHAPE);
	case Capsule: return physics->createShape(physx::PxCapsuleGeometry(m_scale.x, m_scale.y)	   , *m_material, true, physx::PxShapeFlag::eSIMULATION_SHAPE);
	default:	  return nullptr;
	}
}

void Collider::UpdateGeometry() const noexcept
{
	switch (m_shapeType)
	{
	case Box:	  m_shape->setGeometry(physx::PxBoxGeometry(m_scale.x, m_scale.y, m_scale.z)); break;
	case Sphere:  m_shape->setGeometry(physx::PxSphereGeometry(m_scale.x)); break;
	case Capsule: m_shape->setGeometry(physx::PxCapsuleGeometry(m_scale.x, m_scale.y)); break;
	default:	  break;
	}
}

void Collider::AttachToRegidBody() noexcept
{
	if (auto rigidBody = GetOwner()->GetComponent<IRigidBody>())
	{
		if (!m_shape)
		{
			SetShapeType(m_shapeType);
		}

		rigidBody->SetCollider(this);
	}
}

void Collider::DetachFromRegidBody() noexcept
{
	if (auto rigidBody = GetOwner()->GetComponent<IRigidBody>())
	{
		rigidBody->SetCollider(nullptr);

		if (m_shape)
		{
			m_shape->release();
			m_shape = nullptr;
		}
	}
}