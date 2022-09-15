/**
* @file    String.cpp
* @brief   
*
* @date	   2022/08/02 2022年度初版
*/


#include <Windows.h>
#include "String.h"

Wstring ToWstring(StringView str, uint32_t cp /* = CP_ACP */) noexcept
{
	// 変換後の文字列の長さを取得
	int lenght = MultiByteToWideChar(cp, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.data(), -1, nullptr, 0);

	Wstring wstr;
	wstr.reserve(lenght);

	// 文字列変換
	MultiByteToWideChar(cp, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.data(), -1, &wstr[0], lenght);

	return wstr;
}

String ToString(WstringView str, uint32_t cp /* = CP_OEMCP */) noexcept
{
	// 変換後の文字列の長さを取得
	int lenght = WideCharToMultiByte(cp, 0, str.data(), -1, (char*)NULL, 0, NULL, NULL);

	String ret;
	ret.resize(lenght);

	// 文字列変換
	WideCharToMultiByte(cp, 0, str.data(), -1, ret.data(), lenght, NULL, NULL);

	return ret;
}

String ToString(u8StringView str) noexcept
{
	auto wStr = ToWstring(String(str.begin(), str.end()), CP_UTF8);

	return ToString(wStr);
}

u8String ToU8String(StringView str) noexcept
{
	auto wStr = ToWstring(str);
	auto&& temp = ToString(wStr, CP_UTF8);

	return u8String(temp.begin(), temp.end());
}

String GetExt(StringView filePath) noexcept
{
	auto ext_i = filePath.find_last_of(".");
	if (ext_i == StringView::npos)
	{
		return String();
	}

	return String(filePath.substr(ext_i + 1, filePath.size() - ext_i));
}