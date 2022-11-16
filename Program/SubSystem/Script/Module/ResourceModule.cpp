/**
* @file    ResourceModule.cpp
* @brief
*
* @date	   2022/11/14 2022îNìxèâî≈
*/


#include "ModuleHelper.h"
#include "Subsystem/Resource/ResourceManager.h"
#include "Subsystem/Resource/Resources/Audio/AudioClip.h"
#include "Subsystem/Resource/Resources/3DModel/Texture.h"
#include "Subsystem/Resource/Resources/3DModel/Material.h"
#include "Subsystem/Resource/Resources/3DModel/Mesh.h"
#include "Subsystem/Resource/Resources/3DModel/Model.h"
#include "Subsystem/Resource/Resources/Scene/Scene.h"

void SetUpResourceModule()
{
	ResourceHandle* (ResourceManager:: * load)(StringView, StringView, uint32_t) = &ResourceManager::Load;
	ResourceHandle* (ResourceManager:: * loadTexture  )(StringView, uint32_t) = &ResourceManager::Load<Texture>;
	ResourceHandle* (ResourceManager:: * loadMaterial )(StringView, uint32_t) = &ResourceManager::Load<Material>;
	ResourceHandle* (ResourceManager:: * loadMesh	  )(StringView, uint32_t) = &ResourceManager::Load<Mesh>;
	ResourceHandle* (ResourceManager:: * loadModel	  )(StringView, uint32_t) = &ResourceManager::Load<Model>;
	ResourceHandle* (ResourceManager:: * loadAudioClip)(StringView, uint32_t) = &ResourceManager::Load<AudioClip>;
	void (ResourceManager::*unload)(StringView, StringView) = &ResourceManager::Unload;

	PY_CLASS_NOCOPY(ResourceHandle)
		PY_CLASS_DEF(ResourceHandle, IsValid)
		.def("GetScene", &ResourceHandle::GetResource<Scene>, PY_OPAQUE_PTR)
		.def("GetTexture", &ResourceHandle::GetResource<Texture>, PY_OPAQUE_PTR)
		.def("GetMaterial", &ResourceHandle::GetResource<Material>, PY_OPAQUE_PTR)
		.def("GetMesh", &ResourceHandle::GetResource<Mesh>, PY_OPAQUE_PTR)
		.def("GetModel", &ResourceHandle::GetResource<Model>, PY_OPAQUE_PTR)
		.def("GetAudioClip", &ResourceHandle::GetResource<AudioClip>, PY_OPAQUE_PTR)
		PY_CLASS_DEF(ResourceHandle, WaitForLoadComplete);

	PY_CLASS_NOCOPY(ResourceManager)
		.def("Load", load, PY_OPAQUE_PTR)
		.def("LoadTexture", loadTexture, PY_OPAQUE_PTR)
		.def("LoadMaterial", loadMaterial, PY_OPAQUE_PTR)
		.def("LoadMesh", loadMesh, PY_OPAQUE_PTR)
		.def("LoadModel", loadModel, PY_OPAQUE_PTR)
		.def("LoadAudioClip", loadAudioClip, PY_OPAQUE_PTR)
		.def("Unload", unload);

	PY_CLASS_NOCOPY(IResource)
		PY_CLASS_GET("type", IResource, GetType)
		PY_CLASS_MAKE_GET("assetName", IResource, GetAssetName, PY_COPY_CONST_REF)
		PY_CLASS_MAKE_GET("filePath", IResource, GetFilePath, PY_COPY_CONST_REF);
	
	PY_BASE_CLASS_NOCOPY(AudioClip, IResource);

	PY_BASE_CLASS_NOCOPY(Scene, IResource)
		PY_CLASS_DEF_POLICIE(Scene, GetGameObjectByID, PY_OPAQUE_PTR)
		PY_CLASS_DEF_POLICIE(Scene, GetGameObjectByName, PY_OPAQUE_PTR)
		PY_CLASS_DEF_POLICIE(Scene, GetAllGameObjects, PY_COPY_CONST_REF);

	PY_BASE_CLASS_NOCOPY(Texture, IResource);

	PY_BASE_CLASS_NOCOPY(Material, IResource)
		PY_CLASS_MAKE_GETSET("blendMode", Material, GetBlendMode, PY_BY_VALUE, SetBlendMode)
		PY_CLASS_MAKE_GETSET("rasterizerState", Material, GetRasterizerState, PY_BY_VALUE, SetRasterizerState)
		PY_CLASS_MAKE_GETSET("albedo", Material, GetAlbedo, PY_COPY_CONST_REF, SetAlbedo)
		PY_CLASS_MAKE_GETSET("metalic", Material, GetMetallic, PY_BY_VALUE, SetMetallic)
		PY_CLASS_MAKE_GETSET("smooth", Material, GetSmooth, PY_BY_VALUE, SetSmooth)
		PY_CLASS_MAKE_GETSET("emission", Material, GetEmission, PY_COPY_CONST_REF, SetEmission)
		PY_CLASS_DEF(Material, SetTexture);

	PY_BASE_CLASS_NOCOPY(Mesh, IResource)
		PY_CLASS_MAKE_GETSET("material", Mesh, GetMaterial, PY_OPAQUE_PTR, SetMaterial);

	PY_BASE_CLASS_NOCOPY(Model, IResource)
		PY_CLASS_MAKE_GET("mesh", Model, GetMesh, PY_OPAQUE_PTR)
		PY_CLASS_MAKE_GET("meshes", Model, GetAllMeshes, PY_COPY_CONST_REF);
}