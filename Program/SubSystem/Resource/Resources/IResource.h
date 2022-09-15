/**
* @file    IResource.h
* @brief
*
* @date	   2022/08/08 2022�N�x����
*/
#pragma once


class ResourceManager;
struct ResourceData;

class IResource
{
	IN_DEVELOPMENT("Resource�̏������̏���")
	SUPER_CLASS(IResource)

	friend struct ResourceFactory;
public:

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s�� */
	template<class T>
	static T* CreateResource(StringView path) noexcept;
	static IResource* CreateResource(uint32_t type, StringView path) noexcept;

	/** ��ɓƎ��f�[�^�̓ǂݍ��݂��s�� */
	virtual bool Load(StringView path) = 0;

	/** ���\�[�X�f�[�^�t�@�C���X�V�����p */
	virtual void Update() = 0;

	/** ���X���b�h�̃^�X�N�����Ƃ��Ďg�p�����Ȃǂ̎��Ɍ���ď������Ȃ��悤�ɂ��邽�� */
	bool IsErasable() const noexcept;

	Context* GetContext() noexcept;

	/** ResourceData */
	uint32_t GetType() const noexcept;
	String GetAssetName() const noexcept;
	const String& GetFilePath() const noexcept;

protected:

	ResourceData* GetResourceData() noexcept;

protected:

	uint32_t m_type;

	String m_filePath;

	ResourceManager* m_resourceManager;
};

template<class T>
FORCEINLINE T* IResource::CreateResource(StringView path) noexcept
{
	return dynamic_cast<T*>(CreateResource(T::TypeData.Hash, path));
}