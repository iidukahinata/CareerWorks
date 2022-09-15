/**
* @file    ShaderImporter.h
* @brief
*
* @date	   2022/08/07 2022年度初版
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
	* 指定シェーダファイルの asset ファイルの作成及び移動させる。
	*/
	bool CreateShaderData(StringView filePath) noexcept;

private:

	String ConvertProprietaryPath(StringView filePath) noexcept;

private:

	ResourceManager* m_resourceManager;
};