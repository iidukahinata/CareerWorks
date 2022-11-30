/**
* @file	   PhysX.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "PhysX.h"
#include "SubSystem/Scene/Factory/ComponentFactory.h"
#include "SubSystem/Scene/Component/Components/Collider.h"
#include "SubSystem/Scene/Component/Components/RigidBody.h"

physx::PxFilterFlags FilterShader(
    physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
    physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize
)
{
    if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1)) 
    {
        pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
    }
    else
    {
        pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

        if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
        {
            // 衝突時、衝突中、衝突終了時 コールバック呼び出し
            pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
                      |  physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
                      |  physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
        }
    }

    return physx::PxFilterFlag::eDEFAULT;
}

bool PhysX::Initialize()
{
    m_jobs[0].SetFunction([this](double deltaTime) { Simulate(deltaTime); }, FunctionType::StartPhysics);
    m_jobs[1].SetFunction([this](double deltaTime) { ReflectUpdate(); }, FunctionType::EndPhysics);

    SetupPhysXObjects();

    // register component
    ComponentFactory::Register<ICollider, Collider>();
    ComponentFactory::Register<IRigidBody, RigidBody>();

    return true;
}

void PhysX::Shutdown()
{
    for (auto& job : m_jobs)
    {
        job.UnRegisterFromJobSystem();
    }

    if (m_scene)      m_scene->release();
    if (m_physics)    m_physics->release();
    if (m_cooking)    m_cooking->release();
    if (m_dispatcher) m_dispatcher->release();
    if (m_foundation) m_foundation->release();
}

void PhysX::Simulate(double deltaTime) noexcept
{
#ifdef IS_EDITOR
    TIME_LINE_WATCH_START(MainThread, "Physics Simulate");
#endif // IS_EDITOR

    m_scene->simulate(deltaTime);

#ifdef IS_EDITOR
    TIME_LINE_WATCH_END(MainThread);
#endif // IS_EDITOR
}

void PhysX::ReflectUpdate() noexcept
{
#ifdef IS_EDITOR
    TIME_LINE_WATCH_START(MainThread, "Physics Reflect Update");
#endif // IS_EDITOR

    // wait for finish simulate
    m_scene->fetchResults(true);

    for (const auto rigidBody : m_rigidBodys)
    {
        rigidBody->Update();
    }

#ifdef IS_EDITOR
    TIME_LINE_WATCH_END(MainThread);
#endif // IS_EDITOR
}

void PhysX::RegisterRigidBody(IRigidBody* rigidBody)
{
    auto body = dynamic_cast<RigidBody*>(rigidBody);
    ASSERT(body);

    if (auto actor = body->GetRigidActor())
    {
        m_scene->addActor(*actor);
    }

    m_rigidBodys.emplace_back(rigidBody);

    // オブジェクトが登録された場合は更新開始
    for (auto& job : m_jobs)
    {
        job.RegisterToJobSystem();
    }
}

void PhysX::UnRegisterRigidBody(IRigidBody* rigidBody)
{
    auto body = dynamic_cast<RigidBody*>(rigidBody);
    ASSERT(body);

    if (auto actor = body->GetRigidActor())
    {
        m_scene->removeActor(*actor);
    }

    std::erase(m_rigidBodys, rigidBody);

    // 登録オブジェクトが無い場合は更新する必要がない
    if (m_rigidBodys.size() == 0)
    {
        for (auto& job : m_jobs)
        {
            job.UnRegisterFromJobSystem();
        }
    }
}

void PhysX::SetGravity(const Math::Vector3& gravity)
{
    m_gravity = gravity;

    m_scene->setGravity(physx::PxVec3(m_gravity.x, m_gravity.y, m_gravity.z));
}

void PhysX::SetupPhysXObjects() noexcept
{
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_defaultAllocatorCallback, m_defaultErrorCallback);
    ASSERT(m_foundation);

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale());
    ASSERT(m_physics);

    physx::PxTolerancesScale scale;
    physx::PxCookingParams params(scale);
    params.buildGPUData = true;

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, params);
    ASSERT(m_cooking);

    //PxInitExtensions(*m_physics, nullptr);

    constexpr int numUseThread = 4;
    m_dispatcher = physx::PxDefaultCpuDispatcherCreate(numUseThread);
    ASSERT(m_dispatcher);

    physx::PxCudaContextManagerDesc cudaContextManagerDesc;
    m_cudaContextManager = PxCreateCudaContextManager(*m_foundation, cudaContextManagerDesc);
    ASSERT(m_cudaContextManager);

    CreatePhysXScene();
}

void PhysX::CreatePhysXScene() noexcept
{
    physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(m_gravity.x, m_gravity.y, m_gravity.z);
    sceneDesc.cpuDispatcher = m_dispatcher;
    sceneDesc.cudaContextManager = m_cudaContextManager;
    sceneDesc.filterShader = FilterShader;
    sceneDesc.simulationEventCallback = this;

    sceneDesc.flags != physx::PxSceneFlag::eENABLE_PCM;
    sceneDesc.flags != physx::PxSceneFlag::eENABLE_STABILIZATION;
    sceneDesc.flags != physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
    sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
    sceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
    sceneDesc.gpuDynamicsConfig.constraintBufferCapacity *= 2;
    sceneDesc.gpuDynamicsConfig.contactBufferCapacity    *= 2;
    sceneDesc.gpuDynamicsConfig.tempBufferCapacity       *= 2;
    sceneDesc.gpuDynamicsConfig.contactStreamSize        *= 2;
    sceneDesc.gpuDynamicsConfig.patchStreamSize          *= 2;
    sceneDesc.gpuDynamicsConfig.forceStreamCapacity      *= 2;

    m_scene = m_physics->createScene(sceneDesc);
    ASSERT(m_scene);
}

void PhysX::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{

}

void PhysX::onWake(physx::PxActor** actors, physx::PxU32 count)
{

}

void PhysX::onSleep(physx::PxActor** actors, physx::PxU32 count)
{

}

void PhysX::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    for (int i = 0; i < nbPairs; ++i)
    {
        auto& pair = pairs[i];

        auto rigidBodyA = static_cast<RigidBody*>(pairHeader.actors[0]->userData);
        auto rigidBodyB = static_cast<RigidBody*>(pairHeader.actors[0]->userData);

        if (!rigidBodyA || !rigidBodyB)
        {
            continue;
        }

        if(pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            NotifyEvent<CollisionEnterEvent>(rigidBodyA, rigidBodyB);
        }
        if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            NotifyEvent<CollisionStayEvent>(rigidBodyA, rigidBodyB);
        }
        if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            NotifyEvent<CollisionExitEvent>(rigidBodyA, rigidBodyB);
        }
    }
}

void PhysX::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for (int i = 0; i < count; ++i)
    {
        auto& pair = pairs[i];

        auto rigidBodyA = static_cast<RigidBody*>(pair.otherActor->userData);
        auto rigidBodyB = static_cast<RigidBody*>(pair.triggerActor->userData);

        if (!rigidBodyA || !rigidBodyB)
        {
            continue;
        }

        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            NotifyEvent<TriggerEnterEvent>(rigidBodyA, rigidBodyB);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            NotifyEvent<TriggerStayEvent>(rigidBodyA, rigidBodyB);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            NotifyEvent<TriggerExitEvent>(rigidBodyA, rigidBodyB);
        }
    }
}

void PhysX::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{

}

physx::PxPhysics* PhysX::GetDevice() const noexcept
{
    return m_physics;
}