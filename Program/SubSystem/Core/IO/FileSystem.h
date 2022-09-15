/**
* @file    FileSystem.h
* @brief
*
* @date	   2022/08/07 2022�N�x����
*/
#pragma once


#include <filesystem>

/** File�p�֗��֐��R���`�B*/
class FileSystem
{
	IN_DEVELOPMENT("�K�v�ɂȂ莟�揇����`���A�g�����Ă���")
public:

	static void Remove(StringView path) noexcept;

	static bool Copy(StringView oldPath, StringView newPath) noexcept;

	static bool Rename(StringView oldPath, StringView newPath) noexcept;

	static String Canonical(StringView filePath) noexcept;

	static String GetFilePath(StringView filePath) noexcept;

	static Vector<String> GetFilePathsFromDirectory(StringView filePath) noexcept;
};