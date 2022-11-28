/**
 * @file	PhysX.h
 * @brief
 *
 * @date	2022/11/28 2022年度初版
 */
#pragma once


#include "../IPhysics.h"
#include <PxPhysicsAPI.h>


class PhysX : public IPhysics, public physx::PxSimulationEventCallback
{
    COMPLETED_DEVELOPMENT()
    SUB_CLASS(PhysX, IPhysics)
public:

    bool Initialize() override;
    void Shutdown() override;

    void Simulate(double deltaTime) noexcept;
    void ReflectUpdate() noexcept;

    /** RigidBody メソッド */
    void RegisterRigidBody(IRigidBody* rigidBody) override;
    void UnRegisterRigidBody(IRigidBody* rigidBody) override;

    /** Gravity メソッド */
    void SetGravity(const Math::Vector3& gravity) override;

private:

    void SetupPhysXObjects() noexcept;

    void CreatePhysXScene() noexcept;

public:

    /** Event メソッド */
    void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
    void onWake(physx::PxActor** actors, physx::PxU32 count) override;
    void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

public:

    /** アクセス */
    physx::PxPhysics* GetDevice() const noexcept;

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