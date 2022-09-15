/**
* @file    Tools.h
* @brief   �֗��֐�
*
* @date	   2022/08/02 2022�N�x����
*/
#pragma once


/**
* �l���w��{���ɐ؂�グ����
*
* @param value [����] �����O�̒l���w�肷��K�v������܂��B
* @param multiple [����] �w�肳�ꂽ���̒l�̔{���ɒ����B0���w��͏o���Ȃ�
* @return multiple �l�� 0 �̎��A�v�Z���ꂸ value ��Ԃ�
*/
constexpr int AdjustToMultiples(int value, int multiple) noexcept
{
	// 0 ���Z�h�~�̂���
	if (multiple == 0)
		return value;

	const int excess = value % multiple;

	// ���ɒ����ς݂̒l���w�肳�ꂽ�ꍇ�A
	// �l���傫���Ȃ��肪���������ߒ����ς݂��ŏ����𕪂��Ă���
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