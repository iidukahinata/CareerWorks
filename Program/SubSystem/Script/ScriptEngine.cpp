/**
* @file    ScriptEngine.cpp
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/


#include "ScriptEngine.h"
#include "Module/ModuleHelper.h"
#include "SubSystem/Scene/GameObject.h"
#include "SubSystem/Scene/Factory/ComponentFactory.h"
#include "SubSystem/Scene/Component/IRigidBody.h"
#include "SubSystem/Scene/Component/Components/Script.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Script/ScriptInstance.h"

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

bool ScriptEngine::Initialize()
{
	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	InitModule();
	Py_Initialize();

	SetupEventLisneterList();

	// register component
	ComponentFactory::Register<IScript, Script>();

	return true;
}

void ScriptEngine::Shutdown()
{
	// unregister component
	ComponentFactory::UnRegister<IScript>();
}

void ScriptEngine::RebuildAllScript() noexcept
{
	auto resources = m_resourceManager->GetResourcesByType<ScriptInstance>();
	for (const auto& resource : resources)
	{
		auto scriptInstance = dynamic_cast<ScriptInstance*>(resource);

		scriptInstance->Reload();
	}
}

void ScriptEngine::SetupEventLisneterList()
{
	auto notifyHit = [](HitEventType type, IRigidBody* target, IRigidBody* hitObject) {

		if (auto component = target->GetOwner()->FindComponent(IScript::TypeData.Name))
		{
			auto script = dynamic_cast<Script*>(component);
			script->NotifyHit(type, hitObject);
		}
	};

	// collision event
	SetupHitEventLisneter<CollisionEnterEvent>(OnCollisionEnter, notifyHit, CollisionEnter);
	SetupHitEventLisneter<CollisionStayEvent>(OnCollisionStay, notifyHit, CollisionStay);
	SetupHitEventLisneter<CollisionExitEvent>(OnCollisionExit, notifyHit, CollisionExit);

	// trigger event
	SetupHitEventLisneter<TriggerEnterEvent>(OnTriggerEnter, notifyHit, TriggerEnter);
	SetupHitEventLisneter<TriggerStayEvent>(OnTriggerStay, notifyHit, TriggerStay);
	SetupHitEventLisneter<TriggerExitEvent>(OnTriggerExit, notifyHit, TriggerExit);
}