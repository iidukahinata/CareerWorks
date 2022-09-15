/**
* @file    ResourceFactory.h
* @brief
*
* @date	   2022/09/04 2022年度初版
*/


#include "ResourceData.h"

class IResource;

class ResourceCache
{
	COMPLETED_DEVELOPMENT()
public:

	void Clear() noexcept;

	/** Resource */
	void AddResource(const ResourcePath& path, IResource* resource) noexcept;
	IResource* GetResource(const ResourcePath& path) const noexcept;
	void RemoveResource(const ResourcePath& path) noexcept;
	bool HasResource(const ResourcePath& path) noexcept;

	Vector<IResource*> GetResourcesByType(uint32_t type) const noexcept;
	Vector<IResource*> GetResources() const noexcept;

private:

	// * Type -> <ファイル名、リソースオブジェクト>
	using ResourceTypeMap = Unordered_Map<String, UniquePtr<IResource>>;

	// * 使用中リソース
	Map<uint32_t, ResourceTypeMap> m_resources;

	// * このクラス内での排他制御実現用
	std::mutex m_mutex;
};