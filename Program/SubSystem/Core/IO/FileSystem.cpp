/**
* @file    FileSystem.cpp
* @brief
*
* @date	   2022/10/21 2022?N?x????
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
		LOG_ERROR(String("not found file or directory ") + "path => " + path.data());
	}
}

void FileSystem::RemoveAll(StringView path) noexcept
{
	try
	{
		std::filesystem::remove_all(path.data());
	}
	catch (const std::exception&)
	{
		LOG_ERROR(String("not found file or directory ") + "path => " + path.data());
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

Vector<String> FileSystem::GetFilePathsFromDirectory(StringView path) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
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
		LOG_ERROR(String("not found directory ") + "path => " + path.data());
	}

	return filePaths;
}

Vector<String> FileSystem::GetFilePathsRecursiveDirectory(StringView path) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
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
		LOG_ERROR(String("not found directory ") + "path => " + path.data());
	}

	return filePaths;
}

Vector<String> FileSystem::GetDirectorysFromDirectory(StringView path) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
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
		LOG_ERROR(String("not found directory ") + "path => " + path.data());
	}

	return filePaths;
}

Vector<String> FileSystem::GetDirectorysRecursiveDirectory(StringView path) noexcept
{
	Vector<String> filePaths;

	try
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
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
		LOG_ERROR(String("not found directory ") + "path => " + path.data());
	}

	return filePaths;
}

bool FileSystem::IsDirectoryPath(StringView path) noexcept
{
	return std::filesystem::is_directory(path);
}

bool FileSystem::Exists(StringView path) noexcept
{
	return 	std::filesystem::exists(path);
}