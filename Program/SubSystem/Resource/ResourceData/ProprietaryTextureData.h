/**
* @file    ProprietaryTextureData.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "ProprietaryData.h"

struct ProprietaryTextureData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	size_t m_imageCount;
	size_t m_rowPitch;
	size_t m_slicePitch;
	Vector<uint8_t> m_pixels;

	DirectX::TexMetadata m_meta;

public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

public:

	/** DirectTex�f�[�^����Ǝ��f�[�^�ɕϊ��A�e�N�X�`�������Ŏg�p */
	static ProprietaryTextureData ConvertProprietaryData(DirectX::TexMetadata& meta, DirectX::ScratchImage* image) noexcept;

	/** �Ǝ��f�[�^����DirectTex�f�[�^�ɕϊ��A�e�N�X�`���X�V���Ɏg�p */
	Vector<DirectX::Image> ConvertDirectTexData() noexcept;
	static Vector<DirectX::Image> ConvertDirectTexData(ProprietaryTextureData& textureData) noexcept;

	/** �Ǝ��f�[�^�Ƃ��ĕۑ������o�͐�t�@�C���p�X�ɕϊ� */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};