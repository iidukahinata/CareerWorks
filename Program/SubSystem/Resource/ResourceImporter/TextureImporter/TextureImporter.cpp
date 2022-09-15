/**
* @file    TextureImporter.cpp
* @brief
*
* @date	   2022/08/07 2022�N�x����
*/


#include "TextureImporter.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/ResourceData/ProprietaryTextureData.h"

TextureImporter::TextureImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{

}

bool TextureImporter::CreateTextureData(StringView filePath) noexcept
{
	DirectX::TexMetadata meta;
	auto image = std::make_unique<DirectX::ScratchImage>();

	if (!LoadFromFile(GetExt(filePath), ToWstring(filePath), &meta, *image))
	{
		LOG_ERROR(String(filePath) + "���ǂݍ��߂܂���ł����B");
		return false;
	}

	// �e�f�[�^��Ǝ����f���Ƃ��ďo�͂��邽�ߕϊ�
	auto texturePath = ProprietaryTextureData::ConvertProprietaryPath(filePath);
	auto textureData = ProprietaryTextureData::ConvertProprietaryData(meta, image.get());

	m_resourceManager->CreateResourceData("Texture", texturePath);

	return textureData.SaveToFile(texturePath);
}

bool TextureImporter::LoadFromFile(StringView ext, WstringView path, DirectX::TexMetadata* meta, DirectX::ScratchImage& image) noexcept
{
	HRESULT hr = S_FALSE;

	if (ext == "dds")
	{
		hr = DirectX::LoadFromDDSFile(path.data(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, meta, image);
	}
	else if (ext == "tga")
	{
		hr = DirectX::LoadFromTGAFile(path.data(), meta, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(path.data(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, meta, image);
	}

	if (FAILED(hr)) 
	{
		return false;
	}

	return true;
}