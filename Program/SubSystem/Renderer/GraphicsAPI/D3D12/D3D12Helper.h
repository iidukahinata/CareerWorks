/**
* @file    D3D12Helper.cpp
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


/**
* 通常Map等のSTLコンテナではDirectXのDesc構造体等を管理出来ないため、作成。
* これを主に、Smapler等で使用する。
*/
template<class Key, class T>
class D3D12Map
{
	COMPLETED_DEVELOPMENT()
public:

	T Find(const Key& key) noexcept;
	UINT Add(const Key& key, T value) noexcept;
	void Reset() noexcept;

private:

	Map<UINT, T> m_map;
};

template<class Key, class T>
FORCEINLINE T D3D12Map<Key, T>::Find(const Key& key) noexcept
{
	UINT hash = GetHash(key);
	if (m_map.contains(hash))
	{
		return m_map[hash];
	}
	else
	{
		return T();
	}
}

template<class Key, class T>
FORCEINLINE UINT D3D12Map<Key, T>::Add(const Key& key, T value) noexcept
{
	UINT hash = GetHash(key);
	m_map[hash] = value;
	return m_map.size();
}

template<class Key, class T>
FORCEINLINE void D3D12Map<Key, T>::Reset() noexcept
{
	m_map.clear();
}

#define MAX_VERTEX_BUFFER_VIEW 16
#define MAX_RENDER_TARGET 8
#define MAX_VIEWPORT 16
#define MAX_SCISSORRECT 16
#define MAX_CONSTANT_BUFFER_VIEW 16
#define MAX_SHADER_RESOURCE_VIEW 32
#define MAX_UNORDERED_ACCESS_VIEW 16
#define MAX_SAMPLER 16