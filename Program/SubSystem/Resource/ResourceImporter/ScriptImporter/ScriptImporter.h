/**
* @file    ScriptImporter.h
* @brief
*
* @date	   2022/11/18 2022�N�x����
*/
#pragma once


class ResourceManager;

class ScriptImporter
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(ScriptImporter)
public:

	ScriptImporter(ResourceManager* resourceManager);

	/**
	* �w��X�N���v�g�t�@�C���� asset �t�@�C���̍쐬�y�шړ�������B
	*/
	bool CreateScriptData(StringView filePath) noexcept;

private:

	String ConvertProprietaryPath(StringView filePath) noexcept;

private:

	ResourceManager* m_resourceManager;
};