/**
* @file    IResource.cpp
* @brief
*
* @date	   2022/08/08 2022”N“x‰”Å
*/


#include "IResource.h"
#include "SubSystem/Resource/ResourceManager.h"

IResource* IResource::CreateResource(uint32_t type, StringView path) noexcept
{
    static ResourceManager* resourceManager = nullptr;

    if (!resourceManager)
    {
        resourceManager = g_context->GetSubsystem<ResourceManager>();
    }

    return resourceManager->CreateResource(type, path);
}

bool IResource::Load(StringView path)
{
    ASSERT(0);
    return false;
}

void IResource::Update()
{
    ASSERT(0);
};

uint32_t IResource::GetType() const noexcept
{
    return m_type;
}

const String& IResource::GetAssetName() const noexcept
{
    return GetResourceData()->m_assetName;
}

const String& IResource::GetFilePath() const noexcept
{
    return m_filePath;
}

Context* IResource::GetContext() const noexcept
{
    return g_context;
}

ResourceData* IResource::GetResourceData() const noexcept
{
    return m_resourceManager->GetResourceData(m_filePath);
}