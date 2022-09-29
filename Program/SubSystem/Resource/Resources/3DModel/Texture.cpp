/**
* @file    Texture.cpp
* @brief
*
* @date	   2022/09/06 2022îNìxèâî≈
*/


#include "Texture.h"
#include "SubSystem/Resource/ResourceData/ProprietaryTextureData.h"

bool Texture::Load(StringView path)
{
	ProprietaryTextureData textureData;
	textureData.LoadFromFile(path);

	auto images = textureData.ConvertDirectTexData();
	return m_texture.Create(images.data(), images.size(), textureData.m_meta);
}

void Texture::Update()
{
	
}

void* Texture::GetData() noexcept
{
	return &m_texture.GetView();
}

void Texture::PSSet(UINT slot) noexcept
{
	m_texture.PSSet(slot);
}