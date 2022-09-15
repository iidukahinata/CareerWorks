/**
* @file    ProprietaryData.h
* @brief
*
* @date	   2022/08/09 2022îNìxèâî≈
*/
#pragma once


struct ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	virtual ~ProprietaryData() {}

	virtual void Serialized(FileStream* file) const = 0;
	virtual void Deserialization(FileStream* file) = 0;

	bool SaveToFile(StringView path) const noexcept
	{
		FileStream file;
		file.Load(path, OpenMode::Write_Mode);
		if (!file.IsOpen())
		{
			return false;
		}

		Serialized(&file);

		return true;
	}

	bool LoadFromFile(StringView path) noexcept
	{
		FileStream file;
		file.Load(path, OpenMode::Read_Mode);
		if (!file.IsOpen())
		{
			return false;
		}

		Deserialization(&file);

		return true;
	}
};