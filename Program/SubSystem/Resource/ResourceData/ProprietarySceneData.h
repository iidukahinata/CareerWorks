/**
* @file    ProprietarySceneData.h
* @brief
*
* @date	   2022/09/06 2022年度初版
*/
#pragma once


#include "ProprietaryData.h"

struct ProprietarySceneData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	void Serialized(FileStream* file) const override {};
	void Deserialized(FileStream* file) override {};

	/** 独自データとして保存される出力先ファイルパスに変換 */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};