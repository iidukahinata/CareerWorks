/**
* @file  Macro.h
* @brief ���ʓI�ȏ������܂Ƃ߂Ă���
*
* @date	 2022/08/02 2022�N�x����
*/
#pragma once


#include <typeinfo>
#include "Common.h"

// * �T�����ȒP�ɂ��邽�߂ɍ쐬
#define COMPLETED_DEVELOPMENT(...) // �J�������i�����_�ō�Ƃ��s���邱�Ƃ��Ȃ����́j
#define IN_DEVELOPMENT(...)		   // �J�����@�i�i�s���̂��́j
#define WAIT_FOR_DEVELOPMENT(...)  // �J���ҋ@�i���̍�Ƃ�D�悷��ꍇ�Ȃǁj

/**
* typeid �w�肾�ƃN���X�������̂܂܂̕�����Ƃ��Ď擾�o���Ȃ����ߍ쐬�B
* �g�p�p�r�ɂ���邪�A���̃N���X�݂̂̌^��r�͐������Ȃ��B
* �����̗p�r�Ŏg�p����ꍇ�͖��O��r�������ɍs��������萳�m�ȓ����ɂȂ�B
*/
struct ClassTypeData
{
	const StringView Name = nullptr;

	const size_t Hash = 0;

	constexpr ClassTypeData(StringView name, size_t hash) : Name(name), Hash(hash)
	{}

	constexpr ClassTypeData(StringView name) : Name(name), Hash(GetHashFromCRC(name))
	{}

	constexpr bool operator==(const ClassTypeData& typeData) const noexcept
	{
		return Hash == typeData.Hash;
	}
};


#define COPY_PROHIBITED(CLASS)			\
CLASS(const CLASS&) = delete;			\
CLASS& operator=(const CLASS&) = delete;

#define CLASS_DATA(CLASS) \
public: \
static INLINE ClassTypeData TypeData = ClassTypeData(#CLASS, GET_HASH(CLASS));

#define SUPER_CLASS(CLASS)						\
public:											\
CLASS_DATA(CLASS)								\
virtual const ClassTypeData& GetTypeData() const\
{												\
	return TypeData;							\
}												\
const std::type_info& TypeInfo() noexcept		\
{												\
	return typeid(*this);						\
}

#define SUB_CLASS(CLASS)									\
public:														\
CLASS_DATA(CLASS)											\
virtual const ClassTypeData& GetTypeData() const override	\
{															\
	return TypeData;										\
}