/**
* @file    ProprietarySceneData.cpp
* @brief
*
* @date	   2022/09/06 2022年度初版
*/


#include "ProprietarySceneData.h"

void ProprietarySceneData::Serialized(FileStream* file) const
{

}

void ProprietarySceneData::Deserialized(FileStream* file)
{

}

String ProprietarySceneData::ConvertProprietaryPath(StringView filePath) noexcept
{
    // ファイル拡張子を独自ファイル用に変更するため消去
    return (SCENE_DIRECTORY + FileSystem::GetFilePath(filePath.substr(0, filePath.find("."))) + SCENE_EXTENSION);
}