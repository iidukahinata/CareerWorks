/**
* @file  String.h
* @brief
*
* @date	 2022/08/02 2022�N�x����
*/
#pragma once


#include <string_view>

using StringView = std::string_view;
using WstringView = std::wstring_view;

#ifdef __cpp_lib_char8_t
using u8StringView = std::u8string_view;
#endif // __cpp_lib_char8_t

/**
* �����񂩂� wstring �^�ւ̕ϊ�
*
* @param str [����] ��������̕�����ł���K�v������܂��B
*					null �w��͑z�肵�Ă��Ȃ��B
*/
Wstring ToWstring(StringView str, uint32_t cp = CP_ACP) noexcept;

/**
* wstring �^���� string �^�ւ̕ϊ�
*
* @param str [����] ��������̕�����ł���K�v������܂��B
*					null �w��͑z�肵�Ă��Ȃ��B
*/
String ToString(WstringView str, uint32_t cp = CP_OEMCP) noexcept;

#ifdef __cpp_lib_char8_t
String ToString(u8StringView str) noexcept;
u8String ToU8String(StringView str) noexcept;

String ConvertToJapanese(StringView str) noexcept;
#endif // __cpp_lib_char8_t

/**
* �p�X����g���q�擾
*
* @param filePath [����] ��������̃t�@�C�����ł���K�v������܂��B
*/
String GetExt(StringView filePath) noexcept;