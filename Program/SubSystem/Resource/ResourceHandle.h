/**
* @file    ResourceHandle.h
* @brief
*
* @date	   2022/09/08 2022年度初版
*/
#pragma once


class IResource;
class ResourceManager;

/**
* 実行時に使用される、Resource への参照権限を持つクラス
*/
class ResourceHandle
{
	COMPLETED_DEVELOPMENT()

	friend class ResourceManager;
public:

	/**
	* ハンドルの有効性を確認する。
	* これを無視しての Resource 参照は ASSRET 処理が入る。
	*/
	bool IsValid() const noexcept;

	/** 非同期での読み込み優先度を取得 */
	uint32_t GetPriority() const noexcept
	{
		return m_priority;
	}

	/**
	* ハンドルが指す、Resource への参照を行う。
	* ！IsValid() の時に参照すると不正アクセスとして ASSRET する。
	*/
	IResource* GetResource() noexcept;

	template<class T>
	T* GetResource() noexcept
	{
		return dynamic_cast<T*>(GetResource());
	}

	/** 安全な処理待ちだが、atomic な変数へのアクセスがあるため高価 */
	void WaitForLoadComplete() noexcept;

private:

	/** ResourceManager のみがアクセス出来るようにしている */
	ResourceHandle() = default;
	void NotifyCompleteLoad() noexcept;

private:

	// * ハンドルが指す Resource の読み込みが終了したか
	std::atomic_bool m_isCompleteLoad = false;

	// * 非同期読み込み優先度
	uint32_t m_priority = 0;

	// * 読み込む resource data
	ResourceData* m_resourceData = nullptr;

	ResourceManager* m_resourceManager = nullptr;
};