/**
* @file    ScriptImporter.h
* @brief
*
* @date	   2022/11/18 2022年度初版
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
	* 指定スクリプトファイルの asset ファイルの作成及び移動させる。
	*/
	bool CreateScriptData(StringView filePath) noexcept;

private:

	String ConvertProprietaryPath(StringView filePath) noexcept;

private:

	ResourceManager* m_resourceManager;
};