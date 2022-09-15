/**
* @file    AudioImporter.cpp
* @brief
*
* @date	   2022/08/07 2022”N“x‰”Å
*/


#include "AudioImporter.h"
#include "SubSystem/Resource/ResourceManager.h"

AudioImporter::AudioImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{
}

bool AudioImporter::CreateShaderData(StringView filePath) noexcept
{
	auto audioPath = ConvertProprietaryPath(filePath);
	FileSystem::Rename(filePath, audioPath);
	m_resourceManager->CreateResourceData("Audio", audioPath);

	return true;
}

String AudioImporter::ConvertProprietaryPath(StringView filePath) noexcept
{
	return AUDIO_DIRECTORY + FileSystem::GetFilePath(filePath);
}