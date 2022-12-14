/**
* @file    ProprietaryData.h
* @brief
*
* @date	   2022/08/09 2022?N?x????
*/
#pragma once


struct ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	virtual ~ProprietaryData() {}

	virtual void Serialized(FileStream* file) const = 0;
	virtual void Deserialized(FileStream* file) = 0;

public:

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

		Deserialized(&file);

		return true;
	}
};