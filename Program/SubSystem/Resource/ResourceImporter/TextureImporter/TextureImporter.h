/**
* @file    TextureImporter.h
* @brief
*
* @date	   2022/08/09 2022年度初版
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
	* 指定ファイルパスのデータを独自テクスチャデータに変換し作成
	* 生成に成功すれば、指定リソースファイル内に追加される。
	*/
	bool CreateTextureData(StringView filePath) noexcept;

private:

	/** 通常の png : jpg 等テクスチャファイルデータの読み込みを行う */
	bool LoadFromFile(StringView ext, WstringView path, DirectX::TexMetadata* meta, DirectX::ScratchImage& image) noexcept;

private:

	ResourceManager* m_resourceManager;
};