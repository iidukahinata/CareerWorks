/**
* @file    SceneModule.cpp
* @brief
*
* @date	   2022/11/14 2022îNìxèâî≈
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

void SetUpSceneModule()
{
	IComponent* (GameObject::*addComponent)(StringView) = &GameObject::AddComponent;

	PY_CLASS_NOCOPY(GameObject)
		PY_CLASS_DEF(GameObject, Destroy)
		.def("AddComponent", addComponent, PY_OPAQUE_PTR)
		PY_CLASS_DEF(GameObject, RemoveComponent)
		.def("GetAudioListener", &GameObject::GetComponent<AudioListener>, PY_OPAQUE_PTR)
		.def("GetAudioSpeaker", &GameObject::GetComponent<AudioSpeaker>, PY_OPAQUE_PTR)
		.def("GetCamera", &GameObject::GetComponent<Camera>, PY_OPAQUE_PTR)
		.def("GetCollider", &GameObject::GetComponent<Collider>, PY_OPAQUE_PTR)
		.def("GetLight", &GameObject::GetComponent<Light>, PY_OPAQUE_PTR)
		.def("GetPostProcessEffect", &GameObject::GetComponent<PostProcessEffect>, PY_OPAQUE_PTR)
		.def("GetMeshRender", &GameObject::GetComponent<MeshRender>, PY_OPAQUE_PTR)
		.def("GetModelRender", &GameObject::GetComponent<ModelRender>, PY_OPAQUE_PTR)
		.def("GetRigidBody", &GameObject::GetComponent<RigidBody>, PY_OPAQUE_PTR)
		.def("GetScript", &GameObject::GetComponent<Script>, PY_OPAQUE_PTR)
		PY_CLASS_DEF_POLICIE(GameObject, FindComponent, PY_OPAQUE_PTR)
		PY_CLASS_MAKE_GETSET("active", GameObject, GetActive, PY_BY_VALUE, SetActive)
		PY_CLASS_MAKE_GETSET("name", GameObject, GetName, PY_COPY_CONST_REF, SetName)
		PY_CLASS_MAKE_GETSET("id", GameObject, GetID, PY_BY_VALUE, SetID)
		PY_CLASS_MAKE_GET("transform", GameObject, GetTransform, PY_NOT_CONST_REF)
		PY_CLASS_MAKE_GET("scene", GameObject, GetOwner, PY_OPAQUE_PTR)
		PY_CLASS_MAKE_GET("world", GameObject, GetWorld, PY_OPAQUE_PTR);

	PY_CLASS_NOCOPY(World)
		PY_CLASS_DEF(World, LoadScene)
		PY_CLASS_DEF(World, UnloadScene)
		PY_CLASS_DEF(World, ChangeScene)
		PY_CLASS_DEF_POLICIE(World, CreateGameObject, PY_OPAQUE_PTR)
		PY_CLASS_DEF(World, DestroyGameObject)
		PY_CLASS_MAKE_GET("currentScene", World, GetCurrentScene, PY_OPAQUE_PTR)
		PY_CLASS_DEF_POLICIE(World, GetScene, PY_OPAQUE_PTR)
		PY_CLASS_DEF(World, IsGameMode)
		PY_CLASS_DEF(World, IsEditorMode);
}