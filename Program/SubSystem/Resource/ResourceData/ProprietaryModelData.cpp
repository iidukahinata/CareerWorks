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

void ProprietaryModelData::Deserialized(FileStream* file)
{
    file->Read(&m_meshPaths);
}

String ProprietaryModelData::ConvertProprietaryPath(StringView filePath) noexcept
{
    // �t�@�C���g���q��Ǝ��t�@�C���p�ɕύX���邽�ߏ���
    return (MODEL_DIRECTORY + FileSystem::GetFilePath(filePath.substr(0, filePath.find("."))) + MODEL_EXTENSION);
}