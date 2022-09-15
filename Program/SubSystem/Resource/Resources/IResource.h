/**
* @file    IResource.h
* @brief
*
* @date	   2022/08/08 2022年度初版
*/
#pragma once


class ResourceManager;
struct ResourceData;

class IResource
{
	IN_DEVELOPMENT("Resourceの消去時の処理")
	SUPER_CLASS(IResource)

	friend struct ResourceFactory;
public:

	/** asset ファイルと独自データの作成を行う */
	template<class T>
	static T* CreateResource(StringView path) noexcept;
	static IResource* CreateResource(uint32_t type, StringView path) noexcept;

	/** 主に独自データの読み込みを行う */
	virtual bool Load(StringView path) = 0;

	/** リソースデータファイル更新処理用 */
	virtual void Update() = 0;

	/** 他スレッドのタスク処理として使用されるなどの時に誤って消去しないようにするため */
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