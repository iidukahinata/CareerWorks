/**
* @file    ResourceFactory.h
* @brief
*
* @date	   2022/09/04 2022�N�x����
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

	// * Type -> <�t�@�C�����A���\�[�X�I�u�W�F�N�g>
	using ResourceTypeMap = Unordered_Map<String, UniquePtr<IResource>>;

	// * �g�p�����\�[�X
	Map<uint32_t, ResourceTypeMap> m_resources;

	// * ���̃N���X���ł̔r����������p
	std::mutex m_mutex;
};