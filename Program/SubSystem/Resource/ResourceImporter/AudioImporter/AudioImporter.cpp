/**
* @file    AudioImporter.cpp
* @brief
*
* @date	   2022/08/07 2022”N“x‰”Å
*/


#include "AudioImporter.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"

AudioImporter::AudioImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{
}

bool AudioImporter::CreateAudioData(StringView filePath) noexcept
{
	auto audioPath = ConvertProprietaryPath(filePath);
	FileSystem::Copy(filePath, audioPath);

	return m_resourceManager->CreateResourceData<AudioClip>(audioPath);
}

String AudioImporter::ConvertProprietaryPath(StringView filePath) noexcept
{
	return (AUDIO_DIRECTORY + FileSystem::GetFilePath(filePath));
}