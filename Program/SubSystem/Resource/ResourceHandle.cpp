/**
* @file    ResourceHandle.cpp
* @brief
*
* @date	   2022/09/26 2022”N“x‰”Å
*/


#include "ResourceHandle.h"
#include "ResourceManager.h"

bool ResourceHandle::IsValid() const noexcept
{
    return m_isCompleteLoad.load();
}

uint32_t ResourceHandle::GetPriority() const noexcept
{
    return m_priority;
}

IResource* ResourceHandle::GetResource() const noexcept
{
    ASSERT(IsValid());

    return m_resourceManager->GetResource(m_resourceData);
}

void ResourceHandle::WaitForLoadComplete() const noexcept
{
    while (!IsValid())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }
}

void ResourceHandle::NotifyCompleteLoad() noexcept
{
    m_isCompleteLoad.store(true);
}