/**
* @file    ResourceData.cpp
* @brief
*
* @date	   2022/08/07 2022年度初版
*/


#include "ResourceData.h"

ResourcePath::ResourcePath(uint32_t type, String path) :
	m_type(type),
	m_path(path)
{

}

ResourcePath::ResourcePath(StringView type, String path) :
	m_type(GetHashFromCRC(type)),
	m_path(path)
{

}

void ResourcePath::Serialized(FileStream* file) const noexcept
{
	file->Write(m_type);
	file->Write(m_path);
}

void ResourcePath::Deserialized(FileStream* file) noexcept
{
	file->Read(&m_type);
	file->Read(&m_path);
}

void ResourceData::Serialized(FileStream* file) const noexcept
{
	m_resourcePath.Serialized(file);

	file->Write(m_refResourcePaths.size());

	// 参照関係オブジェクト登録
	for (int i = 0; i < m_refResourcePaths.size(); ++i)
	{
		m_refResourcePaths[i].Serialized(file);
	}
}

void ResourceData::Deserialized(FileStream* file) noexcept
{
	m_resourcePath.Deserialized(file);

	size_t size;
	file->Read(&size);

	m_refResourcePaths.resize(size);

	// 参照関係オブジェクト取得
	for (int i = 0; i < m_refResourcePaths.size(); ++i)
	{
		m_refResourcePaths[i].Deserialized(file);
	}
}

bool ResourceData::operator==(const ResourceData& data) noexcept
{
	return m_resourcePath.m_type == data.m_resourcePath.m_type
		&& m_resourcePath.m_path == data.m_resourcePath.m_path;
}

bool ResourceData::operator!=(const ResourceData& data) noexcept
{
	return m_resourcePath.m_type != data.m_resourcePath.m_type
		|| m_resourcePath.m_path != data.m_resourcePath.m_path;
}