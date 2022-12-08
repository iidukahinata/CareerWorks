/**
* @file    RigidBody.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "RigidBody.h"
#include "Collider.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Physics/PhysX/PhysX.h"

void RigidBody::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	file->Write(m_bodyType);
	file->Write(m_mass);
	file->Write(m_useGravity);
	file->Write(m_isKinematic);
	file->Write(m_positionLock);
	file->Write(m_rotationLock);
}

void RigidBody::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	file->Read(&m_bodyType);
	file->Read(&m_mass);
	file->Read(&m_useGravity);
	file->Read(&m_isKinematic);
	file->Read(&m_positionLock);
	file->Read(&m_rotationLock);

	SetBodyType(m_bodyType);
}

void RigidBody::OnInitialize()
{
	m_physics = dynamic_cast<PhysX*>(GetContext()->GetSubsystem<IPhysics>());
	ASSERT(m_physics);
}

void RigidBody::OnStart()
{
	IComponent::OnStart();

	if (GetActive())
	{
		RegisterToPhysics();
	}
}

void RigidBody::OnStop()
{
	IComponent::OnStop();

	if (GetActive())
	{
		UnRegisterFromPhysics();
	}
}

void RigidBody::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);

	if (active)
	{
		RegisterToPhysics();
	}
	else
	{
		UnRegisterFromPhysics();
	}
}

void RigidBody::PreUpdate()
{
	SetPosition(GetTransform().GetPosition());
}

void RigidBody::Update()
{
	auto&& transform = m_actor->getGlobalPose();
	GetTransform().SetPosition(Math::Vector3(transform.p.x, transform.p.y, transform.p.z));
	GetTransform().SetRotation(Math::Quaternion(transform.q.x, transform.q.y, transform.q.z, transform.q.w));
}

void RigidBody::SetCollider(ICollider* collider)
{
	if (!m_actor)
	{
		return;
	}

	if (m_collider == collider)
	{
		return;
	}

	if (m_collider)
	{
		if (auto shape = m_collider->GetShape())
		{
			m_actor->detachShape(*shape);
		}
	}

	m_collider = dynamic_cast<Collider*>(collider);

	if (m_collider)
	{
		if (auto shape = m_collider->GetShape())
		{
			m_actor->attachShape(*shape);
		}
	}
}

void RigidBody::SetBodyType(BodyType type)
{
	if (m_actor)
	{
		SetCollider(nullptr);
		UnRegisterFromPhysics();
	}

	m_bodyType = type;

	if (type == BodyType::Dynamic)
	{
		auto physics = m_physics->GetDevice();
		m_actor = physics->createRigidDynamic(physx::PxTransform(physx::PxVec3()));
	}
	else if(type == BodyType::Static)
	{
		auto physics = m_physics->GetDevice();
		m_actor = physics->createRigidStatic(physx::PxTransform(physx::PxVec3()));
	}
	else
	{
		return;
	}

	ASSERT(m_actor);

	/** set prams */
	SetMass(m_mass);
	SetVelocity(m_velocity);
	SetUseGravity(m_useGravity);
	SetKinematic(m_isKinematic);
	SetPositionLock(m_positionLock);
	SetRotationLock(m_rotationLock);

	if (IsBeginPlay())
	{
		if(m_collider = GetOwner()->GetComponent<Collider>())
		{
			SetCollider(m_collider);
		}

		RegisterToPhysics();
	}
}

BodyType RigidBody::GetBodyType() const
{
	return m_bodyType;
}

void RigidBody::AddForce(const Math::Vector3& force, ForceMode mode /* = ForceMode::Force */)
{
	if (!m_actor && !IsDynamic())
	{
		return;
	}

	auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);

	switch (mode)
	{
	case Force:
		dynamicActor->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eFORCE);
		break;

	case Impulse:
		dynamicActor->addForce(physx::PxVec3(force.x, force.y, force.z), physx::PxForceMode::eIMPULSE);
		break;

	default:
		break;
	}
}

void RigidBody::AddTorque(const Math::Vector3& torque, ForceMode mode /* = ForceMode::Force */)
{
	if (!m_actor && !IsDynamic())
	{
		return;
	}

	auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);

	switch (mode)
	{
	case Force:
		dynamicActor->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eFORCE);
		break;

	case Impulse:
		dynamicActor->addTorque(physx::PxVec3(torque.x, torque.y, torque.z), physx::PxForceMode::eIMPULSE);
		break;

	default:
		break;
	}
}

void RigidBody::SetVelocity(const Math::Vector3& velocity)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		dynamicActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}

	m_velocity = velocity;
}

void RigidBody::AddVelocity(const Math::Vector3& addVelocity)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		auto&& velocity = dynamicActor->getLinearVelocity();

		velocity.x += addVelocity.x;
		velocity.y += addVelocity.y;
		velocity.z += addVelocity.z;

		dynamicActor->setLinearVelocity(velocity);
	}

	m_velocity += addVelocity;
}

Math::Vector3 RigidBody::GetVelocity() const
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		auto&& velocity = dynamicActor->getLinearVelocity();
		return Math::Vector3(velocity.x, velocity.y, velocity.z);
	}

	return Math::Vector3::Zero;
}

void RigidBody::SetAngularVelocity(const Math::Vector3& velocity)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		dynamicActor->setAngularVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
	}
}

void RigidBody::AddAngularVelocity(const Math::Vector3& addVelocity)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		auto&& velocity = dynamicActor->getAngularVelocity();

		velocity.x += addVelocity.x;
		velocity.y += addVelocity.y;
		velocity.z += addVelocity.z;

		dynamicActor->setAngularVelocity(velocity);
	}
}

Math::Vector3 RigidBody::GetAngularVelocity() const
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		auto&& velocity = dynamicActor->getAngularVelocity();
		return Math::Vector3(velocity.x, velocity.y, velocity.z);
	}

	return Math::Vector3::Zero;
}

void RigidBody::SetMass(float mass)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		dynamicActor->setMass(mass);
	}

	m_mass = mass;
}

float RigidBody::GetMass() const
{
	return m_mass;
}

void RigidBody::SetKinematic(bool kinematic)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, kinematic);
	}

	m_isKinematic = kinematic;
}

bool RigidBody::IsKinematic() const
{
	return m_isKinematic;
}

bool RigidBody::IsDynamic() const
{
	return m_bodyType == BodyType::Dynamic;
}

void RigidBody::SetUseGravity(bool useGravity)
{
	if (m_actor)
	{
		m_actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity);
	}

	m_useGravity = useGravity;
}

bool RigidBody::UseGravity() const
{
	return m_useGravity;
}

void RigidBody::SetPositionLock(const Math::Vector3& positionLock)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, !!positionLock.x);
		dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, !!positionLock.y);
		dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, !!positionLock.z);
	}

	m_positionLock = positionLock;
}

const Math::Vector3& RigidBody::GetPositionLock() const
{
	return m_positionLock;
}

void RigidBody::SetRotationLock(const Math::Vector3& rotationLock)
{
	if (m_actor && IsDynamic())
	{
		auto dynamicActor = static_cast<physx::PxRigidDynamic*>(m_actor);
		dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, !!rotationLock.x);
		dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, !!rotationLock.y);
		dynamicActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, !!rotationLock.z);
	}

	m_rotationLock = rotationLock;
}

const Math::Vector3& RigidBody::GetRotationLock() const
{
	return m_rotationLock;
}

void RigidBody::SetPosition(const Math::Vector3& pos) const
{
	if (m_actor)
	{
		auto&& transform = m_actor->getGlobalPose();
		transform.p = physx::PxVec3(pos.x, pos.y, pos.z);

		m_actor->setGlobalPose(transform);
	}
}

void RigidBody::SetRotation(const Math::Quaternion& rot) const
{
	if (m_actor)
	{
		auto&& transform = m_actor->getGlobalPose();
		transform.q = physx::PxQuat(rot.x, rot.y, rot.z, rot.w);
		m_actor->setGlobalPose(transform);
	}
}

physx::PxRigidActor* RigidBody::GetRigidActor() noexcept
{
	return m_actor;
}

void RigidBody::RegisterToPhysics() noexcept
{
	if (!m_actor)
	{
		SetBodyType(m_bodyType);
	}

	SetPosition(GetTransform().GetPosition());
	SetRotation(GetTransform().GetRotation());

	if (!m_isRegister)
	{
		ASSERT(m_actor);

		m_isRegister = true;
		m_physics->RegisterRigidBody(this);
	}
}

void RigidBody::UnRegisterFromPhysics() noexcept
{
	if (m_isRegister)
	{
		m_isRegister = false;
		m_physics->UnRegisterRigidBody(this);
	}

	if (m_actor)
	{
		m_actor->release();
		m_actor = nullptr;
	}
}