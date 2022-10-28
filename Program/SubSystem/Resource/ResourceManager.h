/**
* @file    ResourceManager.h
* @brief
*
* @date	   2022/10/21 2022年度初版
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

/** 非同期での読み込みしか対応していない */
class ResourceManager : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ResourceManager)
public:

	bool Initialize() override;
	void Shutdown() override;

public:

	/** リソースデータの作成 ＆ Cache に登録 */
	template<class T>
	T* CreateResource(StringView path) noexcept;
	IResource* CreateResource(StringView type, StringView path) noexcept;
	IResource* CreateResource(uint32_t type, StringView path) noexcept;

	/** 参照関係にあるリソースも自動的に同時読み込みする */
	template<class T>
	ResourceHandle* Load(StringView name, uint32_t priority = DefaltPriority) noexcept;
	ResourceHandle* Load(StringView type, StringView name, uint32_t priority = DefaltPriority) noexcept;
	ResourceHandle* Load(uint32_t type, StringView name, uint32_t priority = DefaltPriority) noexcept;
	ResourceHandle* Load(ResourceData* resourceData, uint32_t priority = DefaltPriority) noexcept;

	/** 実際には参照数が 0 になったリソースのみが Unload される */
	template<class T>
	void Unload(StringView name) noexcept;
	void Unload(StringView type, StringView name) noexcept;
	void Unload(uint32_t type, StringView name) noexcept;
	void Unload(ResourceData* resourceData) noexcept;

	/** キャッシュから Resource 検索 */
	template<class T>
	T* GetResource(StringView name) noexcept;
	IResource* GetResource(StringView type, StringView name) noexcept;
	IResource* GetResource(uint32_t type, StringView name) noexcept;
	IResource* GetResource(ResourceData* resourceData) const noexcept;
	IResource* GetResource(const ResourcePath& resourcePath) const noexcept;

	/** 呼び出し時に Load されている Resource List 取得 */
	template<class T>
	Vector<IResource*> GetResourcesByType() const noexcept;
	Vector<IResource*> GetResourcesByType(StringView type) noexcept;
	Vector<IResource*> GetResourcesByType(uint32_t type) const noexcept;
	Vector<IResource*> GetResources() const noexcept;

public:

	/** 
	* 新しく asset ファイルの作成を行う。ファイルデータも初期化値がセットされる 
	* @pram name 元データがあるファイルパス
	*/
	template<class T>
	ResourceData* CreateResourceData(StringView path, StringView assetPath = StringView()) noexcept;
	ResourceData* CreateResourceData(StringView type, StringView path, StringView assetPath = StringView()) noexcept;
	ResourceData* CreateResourceData(uint32_t type, StringView path, StringView assetPath = StringView()) noexcept;

	/** 
	* Assetファイルの更新処理のリクエストを行う。
	* 2022/08/07時点では即時更新するようにしているが、動作に支障がある場合はスレッドタスクへの変更も考えられる。
	*/
	void UpdateResourceData(ResourceData* resourceData) noexcept;

	/** ロード時、デバッグ出力等で使用するリソース情報取得関数 */
	template<class T>
	ResourceData* GetResourceData(StringView name) noexcept;
	ResourceData* GetResourceData(StringView type, StringView name) noexcept;
	ResourceData* GetResourceData(uint32_t type, StringView name) noexcept;
	ResourceData* GetResourceData(StringView path) noexcept;

	/** ディレクトリ内の Assets Data を列挙 : Editor の Assets 表示等で使用 */
	void GetResourceDataListFromDirectory(StringView path, Vector<std::pair<String, ResourceData*>>& resourceDatas) noexcept;

public:

	/** assets フォルダから新しくリソースのインポートを行う */
	void ImportAssets() noexcept;
	void ImportAssets(const Vector<String>& filePaths) noexcept;

	/** ResourceTypeList に登録してある全ての ResourceType を列挙します */
	Vector<uint32_t> GetResourceTypes() const noexcept;

	/** Editer 等で使用するため、現在登録されているリソース関係リストを取得。*/
	const Map<uint32_t, Unordered_Map<String, ResourceData>>& GetAllResourceData() const noexcept;
	const Unordered_Map<String, ResourceData>& GetResourceDataListByType(uint32_t type) const noexcept;

	/** アクセス */
	ModelImporter* GetModelImporter() const noexcept { return m_modelImporter.get(); }
	TextureImporter* GetTextureImporter() const noexcept { return m_textureImporter.get(); }
	AudioImporter* GetAudioImporter() const noexcept { return m_audioImporter.get(); }
	ShaderImporter* GetShaderImporter() const noexcept { return m_shaderImporter.get(); }

private:

#if IS_EDITOR
	void StartupListenerObjects() noexcept;
#endif // IS_EDITOR

	/** アプリケーション起動時、リソース関係の構築を行う */
	void DependencyBuilding() noexcept;

	void CreateAsyncLoadList(ResourceData* resourceData, Vector<ResourcePath>& resourcePaths, Set<ResourceData*>& addResourceList) noexcept;

	/** 非同期の Load 処理を新規 Task として登録 */
	void AsyncLoad(ResourceHandle* resourceHandle, const Vector<ResourcePath>& resourcePaths) noexcept;

	/** 指定ファイルのリソースタイプの判別に使用 */
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

	// * Type -> < ClassTypeID : < アセット名 : Resource情報 >>
	Map<uint32_t, Unordered_Map<String, ResourceData>> m_resourceTypeList;

	// * ファイルパスから ResourceData へのアクセス用
	Unordered_Map<String, ResourceData*> m_resourceDataList;

	// * ユーザー側で Handle が放棄されても正常動作するための Map
	Map<ResourceData*, UniquePtr<ResourceHandle>> m_resourceHandles;

	// * 非同期読み込みを行う Job
	Vector<UniquePtr<AsyncJob>> m_asyncJobs;

	// * ロード完了 リソース管理オブジェクト
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