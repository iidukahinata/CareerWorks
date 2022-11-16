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
	COPY_PROHIBITED(IResource)

	friend struct ResourceFactory;
public:

	IResource() = default;
	virtual ~IResource() = default;

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s�� */
	template<class T>
	static T* CreateResource(StringView path) noexcept;
	static IResource* CreateResource(uint32_t type, StringView path) noexcept;

public:

	/** ��ɓƎ��f�[�^�̓ǂݍ��݂��s�� */
	virtual bool Load(StringView path);

	/** ���\�[�X�f�[�^�t�@�C���X�V�����p */
	virtual void Update();

public:

	/** ResourceData */
	uint32_t GetType() const noexcept;
	const String& GetAssetName() const noexcept;
	const String& GetFilePath() const noexcept;

	/** �A�N�Z�X */
	Context* GetContext() const noexcept;

protected:

	ResourceData* GetResourceData() const noexcept;

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