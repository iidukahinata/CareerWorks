/**
* @file    AudioImporter.h
* @brief
*
* @date	   2022/08/07 2022�N�x����
*/
#pragma once


class ResourceManager;

class AudioImporter
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(AudioImporter)
public:

	AudioImporter(ResourceManager* resourceManager);

	/**
	* �w�艹�y�t�@�C���� asset �t�@�C���̍쐬�y�шړ�������B
	*/
	bool CreateShaderData(StringView filePath) noexcept;

private:

	String ConvertProprietaryPath(StringView filePath) noexcept;

private:

	ResourceManager* m_resourceManager;
};