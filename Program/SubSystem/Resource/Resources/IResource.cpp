/**
* @file    IResource.cpp
* @brief
*
* @date	   2022/08/08 2022年度初版
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

bool IResource::IsErasable() const noexcept
{
    return true;
}

uint32_t IResource::GetType() const noexcept
{
    return m_type;
}

String IResource::GetAssetName() const noexcept
{
    StringView path(m_filePath);
    auto noExtPath = path.substr(0, path.find("."));

    // 生成する asset ファイルパスの作成
    return FileSystem::GetFilePath(noExtPath);
}

const String& IResource::GetFilePath() const noexcept
{
    return m_filePath;
}

ResourceData* IResource::GetResourceData() noexcept
{
    return m_resourceManager->GetResourceData(m_filePath);
}

Context* IResource::GetContext() noexcept
{
    return g_context;
}