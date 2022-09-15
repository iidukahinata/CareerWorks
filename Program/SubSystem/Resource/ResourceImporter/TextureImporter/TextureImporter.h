/**
* @file    TextureImporter.h
* @brief
*
* @date	   2022/08/09 2022�N�x����
*/
#pragma once


class ResourceManager;
struct ProprietaryTextureData;

class TextureImporter
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(TextureImporter)
public:

	TextureImporter(ResourceManager* resourceManager);

	/**
	* �w��t�@�C���p�X�̃f�[�^��Ǝ��e�N�X�`���f�[�^�ɕϊ����쐬
	* �����ɐ�������΁A�w�胊�\�[�X�t�@�C�����ɒǉ������B
	*/
	bool CreateTextureData(StringView filePath) noexcept;

private:

	/** �ʏ�� png : jpg ���e�N�X�`���t�@�C���f�[�^�̓ǂݍ��݂��s�� */
	bool LoadFromFile(StringView ext, WstringView path, DirectX::TexMetadata* meta, DirectX::ScratchImage& image) noexcept;

private:

	ResourceManager* m_resourceManager;
};