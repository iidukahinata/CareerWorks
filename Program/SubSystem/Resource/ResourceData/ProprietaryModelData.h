/**
* @file    ProprietaryModelData.h
* @brief
*
* @date	   2022/09/06 2022年度初版
*/
#pragma once


#include "ProprietaryData.h"

class ProprietaryModelData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	// * モデルで使用するメッシュ名
	Vector<String> m_meshPaths;

public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	/** 独自データとして保存される出力先ファイルパスに変換 */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};