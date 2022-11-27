/**
* @file    FileSystem.h
* @brief
*
* @date	   2022/10/21 2022�N�x����
*/
#pragma once


#include <filesystem>

/** File�p�֗��֐��R���`�B*/
class FileSystem
{
	IN_DEVELOPMENT("�K�v�ɂȂ莟�揇����`���A�g�����Ă���")
public:

	static bool CreateDirectory(StringView path) noexcept;

	static void Remove(StringView path) noexcept;

	static void RemoveAll(StringView path) noexcept;

	static bool Copy(StringView oldPath, StringView newPath) noexcept;

	static bool Rename(StringView oldPath, StringView newPath) noexcept;

	static String FindFilePath(StringView directory, StringView path) noexcept;

	static String Canonical(StringView path) noexcept;

	static String GetFilePath(StringView path) noexcept;

	static String GetParentDirectory(StringView path) noexcept;

	static Vector<String> GetFilePathsFromDirectory(StringView path) noexcept;

	static Vector<String> GetFilePathsRecursiveDirectory(StringView path) noexcept;

	static Vector<String> GetDirectorysFromDirectory(StringView path) noexcept;

	static Vector<String> GetDirectorysRecursiveDirectory(StringView path) noexcept;

	static bool IsDirectoryPath(StringView path) noexcept;

	static bool Exists(StringView path) noexcept;
};