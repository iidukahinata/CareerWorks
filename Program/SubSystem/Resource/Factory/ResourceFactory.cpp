/**
* @file    ResourceFactory.cpp
* @brief
*
* @date	   2022/08/08 2022îNìxèâî≈
*/


#include "ResourceFactory.h"
#include "../Resources/IResource.h"
#include "../Resources/Scene/Scene.h"
#include "../Resources/3DModel/Mesh.h"
#include "../Resources/3DModel/Model.h"
#include "../Resources/3DModel/Texture.h"
#include "../Resources/3DModel/Material.h"

#define CASE_CREATE_RESOURCE(CLASS, RESOURCE) case GET_HASH(CLASS): RESOURCE = std::make_unique<CLASS>(); break;

UniquePtr<IResource> ResourceFactory::Create(uint32_t type, StringView path, ResourceManager* resourceManager)
{
	UniquePtr<IResource> resource;

	switch (type)
	{
	CASE_CREATE_RESOURCE(Scene, resource)
	CASE_CREATE_RESOURCE(Mesh, resource)
	CASE_CREATE_RESOURCE(Model, resource)
	CASE_CREATE_RESOURCE(Texture, resource)
	CASE_CREATE_RESOURCE(Material, resource)
	default: break;
	}

	ASSERT(resource);

	resource->m_resourceManager = resourceManager;
	resource->m_type = type;
	resource->m_filePath = path;

	return resource;
}