/**
* @file    Tools.h
* @brief   便利関数
*
* @date	   2022/08/02 2022年度初版
*/
#pragma once


/**
* 値を指定倍数に切り上げ調整
*
* @param value [入力] 調整前の値を指定する必要があります。
* @param multiple [入力] 指定されたこの値の倍数に調整。0を指定は出来ない
* @return multiple 値が 0 の時、計算されず value を返す
*/
constexpr int AdjustToMultiples(int value, int multiple) noexcept
{
	// 0 除算防止のため
	if (multiple == 0)
		return value;

	const int excess = value % multiple;

	// 既に調整済みの値が指定された場合、
	// 値が大きくなる問題があったため調整済みかで処理を分けている
	if (excess == 0)
	{
		return value;
	}
	else
	{
		return value + multiple - excess;
	}
}

template<typename T>
unsigned long GetMsb(const T num) noexcept
{
	unsigned long index = 0;
	_BitScanReverse(&index, static_cast<unsigned long>(num));
	return index;
}

template<typename T>
unsigned long GetLsb(const T num) noexcept
{
	unsigned long index = 0;
	_BitScanForward(&index, static_cast<unsigned long>(num));
	return index;
}