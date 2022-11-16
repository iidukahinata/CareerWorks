/**
* @file    ResourceHandle.h
* @brief
*
* @date	   2022/09/26 2022�N�x����
*/
#pragma once


class IResource;
class ResourceManager;

/**
* ���s���Ɏg�p�����AResource �ւ̎Q�ƌ��������N���X
*/
class ResourceHandle
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(ResourceHandle)

	friend class ResourceManager;
public:

	/** ResourceManager �݂̂��A�N�Z�X�o����悤�ɂ��Ă��� */
	ResourceHandle() = default;

	/**
	* �n���h���̗L�������m�F����B
	* ����𖳎����Ă� Resource �Q�Ƃ� ASSRET ����������B
	*/
	bool IsValid() const noexcept;

	/** �񓯊��ł̓ǂݍ��ݗD��x���擾 */
	uint32_t GetPriority() const noexcept;

	/**
	* �n���h�����w���AResource �ւ̎Q�Ƃ��s���B
	* �IIsValid() �̎��ɎQ�Ƃ���ƕs���A�N�Z�X�Ƃ��� ASSRET ����B
	*/
	template<class T>
	T* GetResource() const noexcept;
	IResource* GetResource() const noexcept;

	/** ���S�ȏ����҂������Aatomic �ȕϐ��ւ̃A�N�Z�X�����邽�ߍ��� */
	void WaitForLoadComplete() const noexcept;

private:

	void NotifyCompleteLoad() noexcept;

private:

	// * �n���h�����w�� Resource �̓ǂݍ��݂��I��������
	std::atomic_bool m_isCompleteLoad = false;

	// * �񓯊��ǂݍ��ݗD��x
	uint32_t m_priority = 0;

	// * �ǂݍ��� resource data
	ResourceData* m_resourceData = nullptr;

	ResourceManager* m_resourceManager = nullptr;
};

template<class T>
T* ResourceHandle::GetResource() const noexcept
{
	return dynamic_cast<T*>(GetResource());
}