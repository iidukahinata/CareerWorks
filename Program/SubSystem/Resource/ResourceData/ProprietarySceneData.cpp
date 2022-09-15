/**
* @file    ProprietarySceneData.cpp
* @brief
*
* @date	   2022/09/06 2022年度初版
*/


#include "ProprietarySceneData.h"
#include "SubSystem/Scene/GameObject.h"

String ProprietarySceneData::ConvertProprietaryPath(StringView filePath) noexcept
{
    // ファイル拡張子を独自ファイル用に変更するため消去
    filePath = filePath.substr(0, filePath.find("."));

    return (SCENE_DIRECTORY + FileSystem::GetFilePath(filePath) + SCENE_EXTENSION);
}