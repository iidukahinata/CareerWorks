/**
* @file    ProprietarySceneData.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "ProprietaryData.h"

struct ProprietarySceneData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	void Serialized(FileStream* file) const override {};
	void Deserialized(FileStream* file) override {};

	/** �Ǝ��f�[�^�Ƃ��ĕۑ������o�͐�t�@�C���p�X�ɕϊ� */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};