/**
* @file    ResourceFactory.cpp
* @brief
*
* @date	   2022/09/04 2022年度初版
*/


#include "ResourceCache.h"
#include "Resources/IResource.h"

void ResourceCache::Clear() noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_resources.clear();
}

void ResourceCache::AddResource(const ResourcePath& path, IResource* resource) noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);

	auto& resourceMap = m_resources[path.m_type];

	// 重複チェック
	ASSERT(!resourceMap.contains(path.m_path));
	resourceMap[path.m_path].reset(resource);
}

IResource* ResourceCache::GetResource(const ResourcePath& path) const noexcept
{
	// 配列外にアクセスしないための判定
	if (!m_resources.contains(path.m_type))
	{
		return nullptr;
	}

	const auto& resourceMap = m_resources.at(path.m_type);

	// 配列外にアクセスしないための判定
	if (!resourceMap.contains(path.m_path))
	{
		return nullptr;
	}

	const auto& resource = resourceMap.at(path.m_path);
	return resource.get();
}

void ResourceCache::RemoveResource(const ResourcePath& path) noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);

	auto& resourceMap = m_resources[path.m_type];
	if (resourceMap.contains(path.m_path))
	{
		resourceMap.erase(path.m_path);
	}
}

bool ResourceCache::HasResource(const ResourcePath& path) noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);

	return m_resources[path.m_type].contains(path.m_path);
}

Vector<IResource*> ResourceCache::GetResourcesByType(uint32_t type) const noexcept
{
	if (!m_resources.contains(type))
	{
		return Vector<IResource*>();
	}

	const auto& resourceList = m_resources.at(type);
	uint32_t size = resourceList.size();

	Vector<IResource*> result;
	result.reserve(size);

	for (const auto& resource : resourceList)
	{
		result.push_back(resource.second.get());
	}

	return result;
}

Vector<IResource*> ResourceCache::GetResources() const noexcept
{
	uint32_t size = 0;
	for (const auto& resources : m_resources)
	{
		size += resources.second.size();
	}

	Vector<IResource*> result;
	result.reserve(size);

	for (const auto& resources : m_resources)
	{
		for (const auto& resource : resources.second)
		{
			result.push_back(resource.second.get());
		}
	}

	return result;
}