/**
* @file    ProprietaryModelData.cpp
* @brief
*
* @date	   2022/08/09 2022年度初版
*/


#include "ProprietaryModelData.h"

void ProprietaryModelData::Serialized(FileStream* file) const
{
    file->Write(m_meshPaths);
}

void ProprietaryModelData::Deserialization(FileStream* file)
{
    file->Read(&m_meshPaths);
}

String ProprietaryModelData::ConvertProprietaryPath(StringView filePath) noexcept
{
    // ファイル拡張子を独自ファイル用に変更するため消去
    filePath = filePath.substr(0, filePath.find("."));

    return (MODEL_DIRECTORY + FileSystem::GetFilePath(filePath) + MODEL_EXTENSION);
}