/**
* @file    ProprietaryModelData.cpp
* @brief
*
* @date	   2022/08/09 2022�N�x����
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
    // �t�@�C���g���q��Ǝ��t�@�C���p�ɕύX���邽�ߏ���
    filePath = filePath.substr(0, filePath.find("."));

    return (MODEL_DIRECTORY + FileSystem::GetFilePath(filePath) + MODEL_EXTENSION);
}