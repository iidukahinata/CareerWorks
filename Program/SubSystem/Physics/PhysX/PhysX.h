/**
 * @file	PhysX.h
 * @brief
 *
 * @date	2022/11/06 2022�N�x����
 */
#pragma once


#include "../Physics.h"
#include <PxPhysicsAPI.h>


class PhysX : public Physics, public physx::PxSimulationEventCallback
{
    COMPLETED_DEVELOPMENT()
    SUB_CLASS(PhysX)
public:

    bool Initialize() override;
    void Shutdown() override;

    void Simulate(double deltaTime) noexcept;
    void ReflectUpdate() noexcept;

    /** RigidBody ���\�b�h */
    void RegisterRigidBody(RigidBody* rigidBody) override;
    void UnRegisterRigidBody(RigidBody* rigidBody) override;

    /** Gravity ���\�b�h */
    void SetGravity(const Math::Vector3& gravity) override;

public:

    /** Event ���\�b�h */
    void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
    void onWake(physx::PxActor** actors, physx::PxU32 count) override;
    void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

public:

    /** �A�N�Z�X */
    physx::PxPhysics* GetDevice() const noexcept;

private:

    void CreateScene() noexcept;

private:

    Array<Job, 2> m_jobs;

    physx::PxDefaultAllocator m_defaultAllocatorCallback;
    physx::PxDefaultErrorCallback m_defaultErrorCallback;

    physx::PxScene* m_scene                           = nullptr;
    physx::PxCooking* m_cooking                       = nullptr;
    physx::PxPhysics* m_physics                       = nullptr;
    physx::PxFoundation* m_foundation                 = nullptr;
    physx::PxDefaultCpuDispatcher* m_dispatcher       = nullptr;
    physx::PxCudaContextManager* m_cudaContextManager = nullptr;
};