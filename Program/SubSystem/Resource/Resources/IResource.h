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
	COPY_PROHIBITED(IResource)

	friend struct ResourceFactory;
public:

	IResource() = default;
	virtual ~IResource() = default;

	/** asset ファイルと独自データの作成を行う */
	template<class T>
	static T* CreateResource(StringView path) noexcept;
	static IResource* CreateResource(uint32_t type, StringView path) noexcept;

public:

	/** 主に独自データの読み込みを行う */
	virtual bool Load(StringView path);

	/** リソースデータファイル更新処理用 */
	virtual void Update();

public:

	/** ResourceData */
	uint32_t GetType() const noexcept;
	const String& GetAssetName() const noexcept;
	const String& GetFilePath() const noexcept;

	/** アクセス */
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