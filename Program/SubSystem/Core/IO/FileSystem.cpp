/**
* @file    FileSystem.cpp
* @brief
*
* @date	   2022/08/07 2022年度初版
*/


#include "FileSystem.h"

void FileSystem::Remove(StringView path) noexcept
{
	try
	{
		std::remove(path.data());
	}
	catch (const std::exception&)
	{
		LOG_ERROR("指定されたファイル、ディレクトリがありません。");
	}
}

bool FileSystem::Copy(StringView oldPath, StringView newPath) noexcept
{
	try
	{
		std::filesystem::copy(oldPath, newPath);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool FileSystem::Rename(StringView oldPath, StringView newPath) noexcept
{
	try
	{
		std::filesystem::rename(oldPath, newPath);
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

String FileSystem::Canonical(StringView file) noexcept
{
	try
	{
		std::filesystem::path buf = std::filesystem::canonical(String("./") + file.data());
		return buf.string().c_str();
	}
	catch (const std::exception&)
	{
		return String();
	}
}

String FileSystem::GetFilePath(StringView filePath) noexcept
{
	try
	{
		return String(filePath.substr(filePath.find_last_of("\\/") + 1, filePath.size()));
	}
	catch (const std::exception&)
	{
		return String(filePath);
	}
}

Vector<String> FileSystem::GetFilePathsFromDirectory(StringView filePath) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(filePath))
		{
			if (entry.is_directory())
			{
				continue;
			}

			filePaths.emplace_back(entry.path().string());
		}
	}
	catch (const std::exception&)
	{
		LOG_ERROR("指定されたディレクトリの検索に失敗しました。");
	}

	return filePaths;
}