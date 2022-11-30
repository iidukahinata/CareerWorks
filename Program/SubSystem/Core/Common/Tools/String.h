/**
* @file  String.h
* @brief
*
* @date	 2022/08/02 2022年度初版
*/
#pragma once


#include <string>

using String = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

using Wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;

using StringView = const String&;
using WstringView = const Wstring&;

#ifdef __cpp_lib_char8_t
using u8String = std::basic_string<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;

using u8StringView = const u8String&;
#endif // __cpp_lib_char8_t

/**
* 文字列から wstring 型への変換
*
* @param str [入力] 何かしらの文字列である必要があります。
*					null 指定は想定していない。
*/
Wstring ToWstring(StringView str, uint32_t cp = CP_ACP) noexcept;

/**
* wstring 型から string 型への変換
*
* @param str [入力] 何かしらの文字列である必要があります。
*					null 指定は想定していない。
*/
String ToString(WstringView str, uint32_t cp = CP_OEMCP) noexcept;

#ifdef __cpp_lib_char8_t
String ToString(u8StringView str) noexcept;
u8String ToU8String(StringView str) noexcept;

String ConvertToJapanese(StringView str) noexcept;
#endif // __cpp_lib_char8_t

/**
* パスから拡張子取得
*
* @param filePath [入力] 何かしらのファイル名である必要があります。
*/
String GetExt(StringView filePath) noexcept;