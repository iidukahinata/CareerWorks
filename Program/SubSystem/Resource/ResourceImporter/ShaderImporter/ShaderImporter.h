/**
* @file    ShaderImporter.h
* @brief
*
* @date	   2022/08/07 2022�N�x����
*/
#pragma once


class ResourceManager;

class ShaderImporter
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(ShaderImporter)
public:

	ShaderImporter(ResourceManager* resourceManager);

	/**
	* �w��V�F�[�_�t�@�C���� asset �t�@�C���̍쐬�y�шړ�������B
	*/
	bool CreateShaderData(StringView filePath) noexcept;

private:

	String ConvertProprietaryPath(StringView filePath) noexcept;

private:

	ResourceManager* m_resourceManager;
};