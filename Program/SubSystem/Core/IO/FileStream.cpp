/**
* @file    FileStream.cpp
* @brief   �t�@�C������N���X
*
* @data	   2022/08/02 2022�N�x����
*/


#include <errno.h>
#include "FileStream.h"
#include "SubSystem/Core/Common/Common.h"

FileStream::FileStream(StringView filePath, OpenMode mode)
{
	Load(filePath, mode);
}

FileStream::~FileStream()
{
	Close();
}

bool FileStream::Load(StringView filePath, OpenMode mode) noexcept
{
	if (IsOpen())
	{
		Close();
	}

	m_openMode = mode;

	errno_t error = 0;
	switch (m_openMode)
	{
	case OpenMode::Txt_Mode:
		error = fopen_s(&m_fp, filePath.data(), "w+");
		break;
	case OpenMode::Read_Mode:
		error = fopen_s(&m_fp, filePath.data(), "rb");
		break;
	case OpenMode::Write_Mode:
		error = fopen_s(&m_fp, filePath.data(), "wb");
		break;
	}

	if (error != 0)
	{
		// �J���Ȃ��������Ƃ� �߂�l�Ŕ��f�o���邽�ߊO���B
		if(error != 2)
		{
			String buf;
			buf.resize(128);
			strerror_s(buf.data(), buf.size(), static_cast<int>(error));
			LOG_ERROR("error :" + String(buf));
		}

		m_openMode = OpenMode::Not_Mode;
		m_fp = nullptr;
	}

	return IsOpen();
}

bool FileStream::CreateFile(StringView filePath, OpenMode mode) noexcept
{
	FILE* fp = nullptr;
	errno_t error = 0;

	switch (mode)
	{
	case OpenMode::Txt_Mode:
		error = fopen_s(&fp, filePath.data(), "w");
		break;
	case OpenMode::Read_Mode:
	case OpenMode::Write_Mode:
		error = fopen_s(&fp, filePath.data(), "wb");
		break;
	}

	if (error == 0)
	{
		// �t�@�C���̍쐬�ɐ���
		if (fp) fclose(fp);
		return true;
	}

	String buf;
	buf.resize(128);
	strerror_s(buf.data(), buf.size(), static_cast<int>(error));
	LOG_ERROR("error :" + String(buf));
	return false;
}

bool FileStream::CreateFileAndLoad(StringView filePath, OpenMode mode) noexcept
{
	if (CreateFile(filePath, mode))
	{
		return Load(filePath, mode);
	}
	return false;
}

void FileStream::Close() noexcept
{
	if (!IsOpen())
		return;

	fclose(m_fp);

	m_fp = nullptr;
	m_openMode = OpenMode::Not_Mode;
}

bool FileStream::IsOpen() const noexcept
{ 
	return m_fp;
}

bool FileStream::IsEof() const noexcept
{
	return feof(m_fp) != 0;
}

OpenMode FileStream::GetMode() const noexcept
{
	return m_openMode;
}