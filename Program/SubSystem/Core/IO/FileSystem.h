/**
* @file    FileSystem.h
* @brief
*
* @date	   2022/10/21 2022年度初版
*/
#pragma once


#include <filesystem>

/** File用便利関数軍を定義。*/
class FileSystem
{
	IN_DEVELOPMENT("必要になり次第順序定義し、拡張していく")
public:

	static bool CreateDirectory(StringView path) noexcept;

	static void Remove(StringView path) noexcept;

	static void RemoveAll(StringView path) noexcept;

	static bool Copy(StringView oldPath, StringView newPath) noexcept;

	static bool Rename(StringView oldPath, StringView newPath) noexcept;

	static String FindFilePath(StringView directory, StringView path) noexcept;

	static String Canonical(StringView filePath) noexcept;

	static String GetFilePath(StringView filePath) noexcept;

	static String GetParentDirectory(StringView directoryPath) noexcept;

	static Vector<String> GetFilePathsFromDirectory(StringView filePath) noexcept;

	static Vector<String> GetFilePathsRecursiveDirectory(StringView filePath) noexcept;

	static Vector<String> GetDirectorysFromDirectory(StringView filePath) noexcept;

	static Vector<String> GetDirectorysRecursiveDirectory(StringView filePath) noexcept;

	static bool IsDirectoryPath(StringView path) noexcept;
};