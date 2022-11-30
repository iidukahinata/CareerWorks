/**
* @file    SceneModule.cpp
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/


#include "ModuleHelper.h"
#include "Subsystem/Scene/World.h"
#include "Subsystem/Scene/GameObject.h"
#include "Subsystem/Scene/TickFunction.h"
#include "Subsystem/Resource/ResourceManager.h"
#include "Subsystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Scene/Component/Components/AudioListener.h"
#include "SubSystem/Scene/Component/Components/AudioSpeaker.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/Collider.h"
#include "SubSystem/Scene/Component/Components/Light.h"
#include "SubSystem/Scene/Component/Components/PostProcessEffect.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Scene/Component/Components/RigidBody.h"
#include "SubSystem/Scene/Component/Components/Script.h"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CreateGameObjectOverloads, CreateGameObject, 0, 1)

void SetUpSceneModule()
{
	IComponent* (GameObject::*addComponent)(StringView) = &GameObject::AddComponent;

	PY_CLASS_NOCOPY(GameObject)
		PY_CLASS_DEF(GameObject, Destroy)
		.def("AddComponent", addComponent, PY_RET_REF)
		PY_CLASS_DEF(GameObject, RemoveComponent)
		.def("GetAudioListener", &GameObject::GetComponent<IAudioListener>, PY_RET_REF)
		.def("GetAudioSpeaker", &GameObject::GetComponent<IAudioSpeaker>, PY_RET_REF)
		.def("GetCamera", &GameObject::GetComponent<ICamera>, PY_RET_REF)
		.def("GetCollider", &GameObject::GetComponent<ICollider>, PY_RET_REF)
		.def("GetLight", &GameObject::GetComponent<ILight>, PY_RET_REF)
		.def("GetPostProcessEffect", &GameObject::GetComponent<IPostProcessEffect>, PY_RET_REF)
		.def("GetMeshRender", &GameObject::GetComponent<IMeshRender>, PY_RET_REF)
		.def("GetModelRender", &GameObject::GetComponent<IModelRender>, PY_RET_REF)
		.def("GetRigidBody", &GameObject::GetComponent<IRigidBody>, PY_RET_REF)
		.def("GetScript", &GameObject::GetComponent<IScript>, PY_RET_REF)
		PY_CLASS_DEF_POLICIE(GameObject, FindComponent, PY_RET_REF)
		PY_CLASS_MAKE_GETSET("active", GameObject, GetActive, PY_BY_VALUE, SetActive)
		PY_CLASS_MAKE_GETSET("name", GameObject, GetName, PY_RET_REF, SetName)
		PY_CLASS_MAKE_GETSET("id", GameObject, GetID, PY_BY_VALUE, SetID)
		PY_CLASS_MAKE_GET("transform", GameObject, GetTransform, PY_RET_REF)
		PY_CLASS_MAKE_GET("scene", GameObject, GetOwner, PY_RET_REF)
		PY_CLASS_MAKE_GET("world", GameObject, GetWorld, PY_RET_REF);

	PY_CLASS_NOCOPY(World)
		PY_CLASS_DEF(World, LoadScene)
		PY_CLASS_DEF(World, UnloadScene)
		PY_CLASS_DEF(World, ChangeScene)
		.def("CreateGameObject", &World::CreateGameObject, PY_RET_REF, CreateGameObjectOverloads())
		PY_CLASS_DEF(World, DestroyGameObject)
		PY_CLASS_MAKE_GET("currentScene", World, GetCurrentScene, PY_RET_REF)
		PY_CLASS_DEF_POLICIE(World, GetScene, PY_RET_REF)
		PY_CLASS_DEF(World, IsGameMode)
		PY_CLASS_DEF(World, IsEditorMode);
}