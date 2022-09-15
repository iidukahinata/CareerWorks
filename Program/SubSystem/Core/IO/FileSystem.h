/**
* @file    FileSystem.h
* @brief
*
* @date	   2022/08/07 2022年度初版
*/
#pragma once


#include <filesystem>

/** File用便利関数軍を定義。*/
class FileSystem
{
	IN_DEVELOPMENT("必要になり次第順序定義し、拡張していく")
public:

	static void Remove(StringView path) noexcept;

	static bool Copy(StringView oldPath, StringView newPath) noexcept;

	static bool Rename(StringView oldPath, StringView newPath) noexcept;

	static String Canonical(StringView filePath) noexcept;

	static String GetFilePath(StringView filePath) noexcept;

	static Vector<String> GetFilePathsFromDirectory(StringView filePath) noexcept;
};