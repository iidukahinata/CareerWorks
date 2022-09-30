/**
* @file    FileSystem.cpp
* @brief
*
* @date	   2022/09/21 2022年度初版
*/


#include "FileSystem.h"

bool FileSystem::CreateDirectory(StringView path) noexcept
{
	return std::filesystem::create_directory(path);
}

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

String FileSystem::FindFilePath(StringView directory, StringView path) noexcept
{
	auto&& paths = GetFilePathsRecursiveDirectory(directory);

	for (auto& str : paths)
	{
		if (str.ends_with(path.data()))
		{
			return str;
		}
	}

	return String();
}

String FileSystem::Canonical(StringView file) noexcept
{
	try
	{
		std::filesystem::path buf = std::filesystem::canonical(file.data());
		return buf.string();
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

String FileSystem::GetParentDirectory(StringView directoryPath) noexcept
{
	try
	{
		return String(directoryPath.substr(0, directoryPath.find_last_of("/\\")));
	}
	catch (const std::exception&)
	{
		return String(directoryPath);
	}
}

Vector<String> FileSystem::GetFilePathsFromDirectory(StringView filePath) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(filePath))
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

Vector<String> FileSystem::GetFilePathsRecursiveDirectory(StringView filePath) noexcept
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

Vector<String> FileSystem::GetDirectorysFromDirectory(StringView filePath) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(filePath))
		{
			if (!entry.is_directory())
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

Vector<String> FileSystem::GetDirectorysRecursiveDirectory(StringView filePath) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(filePath))
		{
			if (!entry.is_directory())
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
