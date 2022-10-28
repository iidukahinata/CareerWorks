/**
* @file    ResourceManager.h
* @brief
*
* @date	   2022/10/21 2022�N�x����
*/
#pragma once


#include "ResourceData.h"
#include "ResourceCache.h"
#include "ResourceHandle.h"
#include "Resources/IResource.h"
#include "ResourceImporter/ModelImporter/ModelImporter.h"
#include "ResourceImporter/TextureImporter/TextureImporter.h"
#include "ResourceImporter/AudioImporter/AudioImporter.h"
#include "ResourceImporter/ShaderImporter/ShaderImporter.h"

enum PrioritySetting
{
	DefaltPriority = 128,
};

/** �񓯊��ł̓ǂݍ��݂����Ή����Ă��Ȃ� */
class ResourceManager : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ResourceManager)
public:

	bool Initialize() override;
	void Shutdown() override;

public:

	/** ���\�[�X�f�[�^�̍쐬 �� Cache �ɓo�^ */
	template<class T>
	T* CreateResource(StringView path) noexcept;
	IResource* CreateResource(StringView type, StringView path) noexcept;
	IResource* CreateResource(uint32_t type, StringView path) noexcept;

	/** �Q�Ɗ֌W�ɂ��郊�\�[�X�������I�ɓ����ǂݍ��݂��� */
	template<class T>
	ResourceHandle* Load(StringView name, uint32_t priority = DefaltPriority) noexcept;
	ResourceHandle* Load(StringView type, StringView name, uint32_t priority = DefaltPriority) noexcept;
	ResourceHandle* Load(uint32_t type, StringView name, uint32_t priority = DefaltPriority) noexcept;
	ResourceHandle* Load(ResourceData* resourceData, uint32_t priority = DefaltPriority) noexcept;

	/** ���ۂɂ͎Q�Ɛ��� 0 �ɂȂ������\�[�X�݂̂� Unload ����� */
	template<class T>
	void Unload(StringView name) noexcept;
	void Unload(StringView type, StringView name) noexcept;
	void Unload(uint32_t type, StringView name) noexcept;
	void Unload(ResourceData* resourceData) noexcept;

	/** �L���b�V������ Resource ���� */
	template<class T>
	T* GetResource(StringView name) noexcept;
	IResource* GetResource(StringView type, StringView name) noexcept;
	IResource* GetResource(uint32_t type, StringView name) noexcept;
	IResource* GetResource(ResourceData* resourceData) const noexcept;
	IResource* GetResource(const ResourcePath& resourcePath) const noexcept;

	/** �Ăяo������ Load ����Ă��� Resource List �擾 */
	template<class T>
	Vector<IResource*> GetResourcesByType() const noexcept;
	Vector<IResource*> GetResourcesByType(StringView type) noexcept;
	Vector<IResource*> GetResourcesByType(uint32_t type) const noexcept;
	Vector<IResource*> GetResources() const noexcept;

public:

	/** 
	* �V���� asset �t�@�C���̍쐬���s���B�t�@�C���f�[�^���������l���Z�b�g����� 
	* @pram name ���f�[�^������t�@�C���p�X
	*/
	template<class T>
	ResourceData* CreateResourceData(StringView path, StringView assetPath = StringView()) noexcept;
	ResourceData* CreateResourceData(StringView type, StringView path, StringView assetPath = StringView()) noexcept;
	ResourceData* CreateResourceData(uint32_t type, StringView path, StringView assetPath = StringView()) noexcept;

	/** 
	* Asset�t�@�C���̍X�V�����̃��N�G�X�g���s���B
	* 2022/08/07���_�ł͑����X�V����悤�ɂ��Ă��邪�A����Ɏx�Ⴊ����ꍇ�̓X���b�h�^�X�N�ւ̕ύX���l������B
	*/
	void UpdateResourceData(ResourceData* resourceData) noexcept;

	/** ���[�h���A�f�o�b�O�o�͓��Ŏg�p���郊�\�[�X���擾�֐� */
	template<class T>
	ResourceData* GetResourceData(StringView name) noexcept;
	ResourceData* GetResourceData(StringView type, StringView name) noexcept;
	ResourceData* GetResourceData(uint32_t type, StringView name) noexcept;
	ResourceData* GetResourceData(StringView path) noexcept;

	/** �f�B���N�g������ Assets Data ��� : Editor �� Assets �\�����Ŏg�p */
	void GetResourceDataListFromDirectory(StringView path, Vector<std::pair<String, ResourceData*>>& resourceDatas) noexcept;

public:

	/** assets �t�H���_����V�������\�[�X�̃C���|�[�g���s�� */
	void ImportAssets() noexcept;
	void ImportAssets(const Vector<String>& filePaths) noexcept;

	/** ResourceTypeList �ɓo�^���Ă���S�Ă� ResourceType ��񋓂��܂� */
	Vector<uint32_t> GetResourceTypes() const noexcept;

	/** Editer ���Ŏg�p���邽�߁A���ݓo�^����Ă��郊�\�[�X�֌W���X�g���擾�B*/
	const Map<uint32_t, Unordered_Map<String, ResourceData>>& GetAllResourceData() const noexcept;
	const Unordered_Map<String, ResourceData>& GetResourceDataListByType(uint32_t type) const noexcept;

	/** �A�N�Z�X */
	ModelImporter* GetModelImporter() const noexcept { return m_modelImporter.get(); }
	TextureImporter* GetTextureImporter() const noexcept { return m_textureImporter.get(); }
	AudioImporter* GetAudioImporter() const noexcept { return m_audioImporter.get(); }
	ShaderImporter* GetShaderImporter() const noexcept { return m_shaderImporter.get(); }

private:

#if IS_EDITOR
	void StartupListenerObjects() noexcept;
#endif // IS_EDITOR

	/** �A�v���P�[�V�����N�����A���\�[�X�֌W�̍\�z���s�� */
	void DependencyBuilding() noexcept;

	void CreateAsyncLoadList(ResourceData* resourceData, Vector<ResourcePath>& resourcePaths, Set<ResourceData*>& addResourceList) noexcept;

	/** �񓯊��� Load ������V�K Task �Ƃ��ēo�^ */
	void AsyncLoad(ResourceHandle* resourceHandle, const Vector<ResourcePath>& resourcePaths) noexcept;

	/** �w��t�@�C���̃��\�[�X�^�C�v�̔��ʂɎg�p */
	bool IsModelFilePath(StringView path) const noexcept;
	bool IsTextureFilePath(StringView path) const noexcept;
	bool IsAudioFilePath(StringView path) const noexcept;
	bool IsShaderFilePath(StringView path) const noexcept;
	bool IsProprietaryFilePath(StringView path) const noexcept;

	String GetAssetPath(StringView assetName) const noexcept;
	String ToAssetDirectory(StringView resourcePath) const noexcept;

private:

	// * importer objects
	UniquePtr<ModelImporter> m_modelImporter;
	UniquePtr<TextureImporter> m_textureImporter;
	UniquePtr<AudioImporter> m_audioImporter;
	UniquePtr<ShaderImporter> m_shaderImporter;

	// * Type -> < ClassTypeID : < �A�Z�b�g�� : Resource��� >>
	Map<uint32_t, Unordered_Map<String, ResourceData>> m_resourceTypeList;

	// * �t�@�C���p�X���� ResourceData �ւ̃A�N�Z�X�p
	Unordered_Map<String, ResourceData*> m_resourceDataList;

	// * ���[�U�[���� Handle ����������Ă����퓮�삷�邽�߂� Map
	Map<ResourceData*, UniquePtr<ResourceHandle>> m_resourceHandles;

	// * �񓯊��ǂݍ��݂��s�� Job
	Vector<UniquePtr<AsyncJob>> m_asyncJobs;

	// * ���[�h���� ���\�[�X�Ǘ��I�u�W�F�N�g
	UniquePtr<ResourceCache> m_resourceCache;

#if IS_EDITOR
	EventListener m_deleteObjectListener;
	EventListener m_importResourceListener;
#endif // IS_EDITOR
};

template<class T>
FORCEINLINE T* ResourceManager::CreateResource(StringView path) noexcept
{
	return dynamic_cast<T*>(CreateResource(T::TypeData.Hash, path));
}

template<class T>
FORCEINLINE ResourceHandle* ResourceManager::Load(StringView name, uint32_t priority /* = DefaltPriority */) noexcept
{
	return Load(T::TypeData.Hash, name, priority);
}

template<class T>
FORCEINLINE void ResourceManager::Unload(StringView name) noexcept
{
	Unload(T::TypeData.Hash, name);
}

template<class T>
FORCEINLINE T* ResourceManager::GetResource(StringView name) noexcept
{
	return dynamic_cast<T*>(GetResource(T::TypeData.Hash, name));
}

template<class T>
FORCEINLINE Vector<IResource*> ResourceManager::GetResourcesByType() const noexcept
{
	return GetResourcesByType(T::TypeData.Hash);
}

template<class T>
FORCEINLINE ResourceData* ResourceManager::CreateResourceData(StringView path, StringView assetPath /* = StringView() */) noexcept
{
	return CreateResourceData(T::TypeData.Hash, path, assetPath);
}

template<class T>
FORCEINLINE ResourceData* GetResourceData(StringView name) noexcept
{
	return GetResourceData(T::TypeData.Hash, name);
}