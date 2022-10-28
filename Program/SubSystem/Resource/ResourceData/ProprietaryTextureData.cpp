/**
* @file    ProprietaryTextureData.cpp
* @brief
*
* @date	   2022/08/09 2022年度初版
*/


#include "ProprietaryTextureData.h"

void ProprietaryTextureData::Serialized(FileStream* file) const
{
	file->Write(m_imageCount);
	file->Write(m_rowPitch);
	file->Write(m_slicePitch);
	file->Write(m_pixels);
	file->Write(m_meta);
}

void ProprietaryTextureData::Deserialized(FileStream* file)
{
	file->Read(&m_imageCount);
	file->Read(&m_rowPitch);
	file->Read(&m_slicePitch);
	file->Read(&m_pixels);
	file->Read(&m_meta);
}

ProprietaryTextureData ProprietaryTextureData::ConvertProprietaryData(DirectX::TexMetadata& meta, DirectX::ScratchImage* image) noexcept
{
	ProprietaryTextureData textureData;

	textureData.m_imageCount = image->GetImageCount();
	textureData.m_meta		 = meta;
	textureData.m_rowPitch	 = image->GetImages()->rowPitch;
	textureData.m_slicePitch = image->GetImages()->slicePitch;

	auto size = image->GetPixelsSize();
	textureData.m_pixels.resize(size);

	size *= sizeof(uint8_t);
	memcpy_s(textureData.m_pixels.data(), size, image->GetImages()->pixels, size);
	
	return textureData;
}

Vector<DirectX::Image> ProprietaryTextureData::ConvertDirectTexData() noexcept
{
	return ConvertDirectTexData(*this);
}

Vector<DirectX::Image> ProprietaryTextureData::ConvertDirectTexData(ProprietaryTextureData& textureData) noexcept
{
	Vector<DirectX::Image> images;

	images.resize(textureData.m_imageCount);
	for (int i = 0; i < textureData.m_imageCount; ++i)
	{
		images[i].width		 = textureData.m_meta.width;
		images[i].height	 = textureData.m_meta.height;
		images[i].format	 = textureData.m_meta.format;
		images[i].rowPitch	 = textureData.m_rowPitch;
		images[i].slicePitch = textureData.m_slicePitch;
		images[i].pixels	 = &textureData.m_pixels[textureData.m_slicePitch * i];
	}

	return images;
}

String ProprietaryTextureData::ConvertProprietaryPath(StringView filePath) noexcept
{
	// ファイル拡張子を独自ファイル用に変更するため消去
	filePath = filePath.substr(0, filePath.find("."));

	return (TEXTURE_DIRECTORY + FileSystem::GetFilePath(filePath) + TEXTURE_EXTENSION);
}