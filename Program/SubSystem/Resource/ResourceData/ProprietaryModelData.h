/**
* @file    ProprietaryModelData.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "ProprietaryData.h"

class ProprietaryModelData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	// * ���f���Ŏg�p���郁�b�V����
	Vector<String> m_meshPaths;

public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	/** �Ǝ��f�[�^�Ƃ��ĕۑ������o�͐�t�@�C���p�X�ɕϊ� */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};