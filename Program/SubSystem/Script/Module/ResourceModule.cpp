/**
* @file    ResourceModule.cpp
* @brief
*
* @date	   2022/11/30 2022îNìxèâî≈
*/


#include "ModuleHelper.h"
#include "Subsystem/Resource/ResourceManager.h"
#include "Subsystem/Resource/Resources/Audio/AudioClip.h"
#include "Subsystem/Resource/Resources/3DModel/Texture.h"
#include "Subsystem/Resource/Resources/3DModel/Material.h"
#include "Subsystem/Resource/Resources/3DModel/Mesh.h"
#include "Subsystem/Resource/Resources/3DModel/Model.h"
#include "Subsystem/Resource/Resources/Scene/Scene.h"
#include "Subsystem/Resource/Resources/Script/ScriptInstance.h"
#include "Subsystem/Scene/Component/IScript.h"

void SetUpResourceModule()
{
	ResourceHandle* (ResourceManager:: * load		  )(StringView, StringView, uint32_t) = &ResourceManager::Load;
	ResourceHandle* (ResourceManager:: * loadTexture  )(StringView, uint32_t) = &ResourceManager::Load<Texture>;
	ResourceHandle* (ResourceManager:: * loadMaterial )(StringView, uint32_t) = &ResourceManager::Load<Material>;
	ResourceHandle* (ResourceManager:: * loadMesh	  )(StringView, uint32_t) = &ResourceManager::Load<Mesh>;
	ResourceHandle* (ResourceManager:: * loadModel	  )(StringView, uint32_t) = &ResourceManager::Load<Model>;
	ResourceHandle* (ResourceManager:: * loadAudioClip)(StringView, uint32_t) = &ResourceManager::Load<AudioClip>;
	ResourceHandle* (ResourceManager:: * loadScript   )(StringView, uint32_t) = &ResourceManager::Load<ScriptInstance>;
	Texture*		(ResourceManager:: * getTexture	  )(StringView) = &ResourceManager::GetResource<Texture>;
	Material*		(ResourceManager:: * getMaterial  )(StringView) = &ResourceManager::GetResource<Material>;
	Mesh*			(ResourceManager:: * getMesh	  )(StringView) = &ResourceManager::GetResource<Mesh>;
	Model*			(ResourceManager:: * getModel	  )(StringView) = &ResourceManager::GetResource<Model>;
	AudioClip*		(ResourceManager:: * getAudioClip )(StringView) = &ResourceManager::GetResource<AudioClip>;
	ScriptInstance* (ResourceManager:: * getScript	  )(StringView) = &ResourceManager::GetResource<ScriptInstance>;
	void			(ResourceManager::*unload		  )(StringView, StringView)  = &ResourceManager::Unload;

	PY_CLASS_NOCOPY(ResourceHandle)
		PY_CLASS_DEF(ResourceHandle, IsValid)
		.def("GetScene", &ResourceHandle::GetResource<Scene>, PY_RET_REF)
		.def("GetTexture", &ResourceHandle::GetResource<Texture>, PY_RET_REF)
		.def("GetMaterial", &ResourceHandle::GetResource<Material>, PY_RET_REF)
		.def("GetMesh", &ResourceHandle::GetResource<Mesh>, PY_RET_REF)
		.def("GetModel", &ResourceHandle::GetResource<Model>, PY_RET_REF)
		.def("GetAudioClip", &ResourceHandle::GetResource<AudioClip>, PY_RET_REF)
		.def("GetScript", &ResourceHandle::GetResource<ScriptInstance>, PY_RET_REF)
		PY_CLASS_DEF(ResourceHandle, WaitForLoadComplete);

	PY_CLASS_NOCOPY(ResourceManager)
		.def("Load", load, PY_RET_REF)
		.def("LoadTexture", loadTexture, PY_RET_REF)
		.def("LoadMaterial", loadMaterial, PY_RET_REF)
		.def("LoadMesh", loadMesh, PY_RET_REF)
		.def("LoadModel", loadModel, PY_RET_REF)
		.def("LoadAudioClip", loadAudioClip, PY_RET_REF)
		.def("LoadScript", loadScript, PY_RET_REF)
		.def("GetTexture", getTexture, PY_RET_REF)
		.def("GetMaterial", getMaterial, PY_RET_REF)
		.def("GetMesh", getMesh, PY_RET_REF)
		.def("GetModel", getModel, PY_RET_REF)
		.def("GetAudioClip", getAudioClip, PY_RET_REF)
		.def("GetScript", getScript, PY_RET_REF)
		.def("Unload", unload);

	PY_CLASS_NOCOPY(IResource)
		PY_CLASS_GET("type", IResource, GetType)
		PY_CLASS_MAKE_GET("assetName", IResource, GetAssetName, PY_RET_REF)
		PY_CLASS_MAKE_GET("filePath", IResource, GetFilePath, PY_RET_REF);
	
	PY_BASE_CLASS_NOCOPY(AudioClip, IResource);

	PY_BASE_CLASS_NOCOPY(Scene, IResource)
		PY_CLASS_DEF_POLICIE(Scene, GetGameObjectByID, PY_RET_REF)
		PY_CLASS_DEF_POLICIE(Scene, GetGameObjectByName, PY_RET_REF)
		PY_CLASS_DEF_POLICIE(Scene, GetAllGameObjects, PY_RET_REF);

	PY_BASE_CLASS_NOCOPY(Texture, IResource);

	PY_BASE_CLASS_NOCOPY(Material, IResource)
		PY_CLASS_MAKE_GETSET("blendMode", Material, GetBlendMode, PY_BY_VALUE, SetBlendMode)
		PY_CLASS_MAKE_GETSET("rasterizerState", Material, GetRasterizerState, PY_BY_VALUE, SetRasterizerState)
		PY_CLASS_MAKE_GETSET("albedo", Material, GetAlbedo, PY_RET_REF, SetAlbedo)
		PY_CLASS_MAKE_GETSET("metalic", Material, GetMetallic, PY_BY_VALUE, SetMetallic)
		PY_CLASS_MAKE_GETSET("smooth", Material, GetSmooth, PY_BY_VALUE, SetSmooth)
		PY_CLASS_MAKE_GETSET("emission", Material, GetEmission, PY_RET_REF, SetEmission)
		PY_CLASS_DEF(Material, SetTexture);

	PY_BASE_CLASS_NOCOPY(Mesh, IResource)
		PY_CLASS_MAKE_GETSET("material", Mesh, GetMaterial, PY_RET_REF, SetMaterial);

	PY_BASE_CLASS_NOCOPY(Model, IResource)
		PY_CLASS_MAKE_GET("mesh", Model, GetMesh, PY_RET_REF)
		PY_CLASS_MAKE_GET("meshes", Model, GetAllMeshes, PY_RET_REF);

	PY_BASE_CLASS_NOCOPY(ScriptInstance, IResource)
		PY_CLASS_DEF(ScriptInstance, GetInstance);
}