/**
* @file    AudioImporter.h
* @brief
*
* @date	   2022/08/07 2022年度初版
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
	* 指定音楽ファイルの asset ファイルの作成及び移動させる。
	*/
	bool CreateShaderData(StringView filePath) noexcept;

private:

	String ConvertProprietaryPath(StringView filePath) noexcept;

private:

	ResourceManager* m_resourceManager;
};