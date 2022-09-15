/**
* @file    ProprietarySceneData.cpp
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/


#include "ProprietarySceneData.h"
#include "SubSystem/Scene/GameObject.h"

String ProprietarySceneData::ConvertProprietaryPath(StringView filePath) noexcept
{
    // �t�@�C���g���q��Ǝ��t�@�C���p�ɕύX���邽�ߏ���
    filePath = filePath.substr(0, filePath.find("."));

    return (SCENE_DIRECTORY + FileSystem::GetFilePath(filePath) + SCENE_EXTENSION);
}