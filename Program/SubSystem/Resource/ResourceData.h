/**
* @file    ResourceData.h
* @brief
*
* @date	   2022/08/07 2022年度初版
*/
#pragma once


struct ResourcePath
{
	// 各リソース識別番号
	uint32_t m_type;

	// リソースデータがあるファイルパス
	String m_path;

public:

	ResourcePath() = default;
	ResourcePath(uint32_t type, String path);
	ResourcePath(StringView type, String path);

	void Serialized(FileStream* file) const noexcept;
	void Deserialization(FileStream* file) noexcept;
};

struct ResourceData
{
	String m_assetName;

	String m_assetFullPath;

	ResourcePath m_resourcePath;

	// * 参照関係のリソースデータ (自動ロードのために使用)
	Vector<ResourcePath> m_refResourcePaths;
	
	// * 参照関係から複数使用されるなどロードされている数
	uint32_t m_numLoading = 0;

public:

	void Serialized(FileStream* file) const noexcept;
	void Deserialization(FileStream* file) noexcept;

	bool operator==(const ResourceData& data) noexcept;
	bool operator!=(const ResourceData& data) noexcept;
};