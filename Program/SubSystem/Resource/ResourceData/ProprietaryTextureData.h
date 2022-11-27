/**
* @file    ProprietaryTextureData.h
* @brief
*
* @date	   2022/09/06 2022年度初版
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

	/** DirectTexデータから独自データに変換、テクスチャ生成で使用 */
	static ProprietaryTextureData ConvertProprietaryData(DirectX::TexMetadata& meta, DirectX::ScratchImage* image) noexcept;

	/** 独自データからDirectTexデータに変換、テクスチャ更新時に使用 */
	Vector<DirectX::Image> ConvertDirectTexData() noexcept;
	static Vector<DirectX::Image> ConvertDirectTexData(ProprietaryTextureData& textureData) noexcept;

	/** 独自データとして保存される出力先ファイルパスに変換 */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};