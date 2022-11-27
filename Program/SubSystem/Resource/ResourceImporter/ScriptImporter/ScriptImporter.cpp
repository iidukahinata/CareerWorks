/**
* @file    ScriptImporter.cpp
* @brief
*
* @date	   2022/11/18 2022”N“x‰”Å
*/


#include "ScriptImporter.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Script/ScriptInstance.h"

ScriptImporter::ScriptImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{

}

bool ScriptImporter::CreateScriptData(StringView filePath) noexcept
{
	auto scriptPath = ConvertProprietaryPath(filePath);
	FileSystem::Copy(filePath, scriptPath);

	if (GetExt(filePath) == "pyd")
	{
		return true;
	}

	return m_resourceManager->CreateResourceData<ScriptInstance>(scriptPath);
}

String ScriptImporter::ConvertProprietaryPath(StringView filePath) noexcept
{
	return (SCRIPT_DIRECTORY + FileSystem::GetFilePath(filePath));
}