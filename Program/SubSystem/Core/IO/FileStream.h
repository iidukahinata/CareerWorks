/**
* @file    FileStream.h
* @brief   ファイル操作クラス
*
* @data	   2022/08/02 2022年度初版
* @note
*  書き読み速度向上のため c 言語での処理に修正。
*  fscanf 操作関数は作成していません。
*/
#pragma once


#include <fstream>

enum class OpenMode
{
	Txt_Mode,
	Read_Mode,
	Write_Mode,
	Not_Mode,
};

/**
* ほとんどの関数が OpenMode によって内部実装が変わる string 型で特殊化されています。
*/
class FileStream
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(FileStream)
public:

	FileStream() = default;
	FileStream(StringView filePath, OpenMode mode);

	~FileStream();

	bool Load(StringView filePath, OpenMode mode) noexcept;
	void Close() noexcept;

	/** もし同じ名前のFileがある場合はデータを上書きして作成する。*/
	bool CreateFile(StringView filePath, OpenMode mode) noexcept;
	bool CreateFileAndLoad(StringView filePath, OpenMode mode) noexcept;

	//== Write ==========================================
	template<class T>
	void Write(T* data, const size_t& size) noexcept;

	template<class T>
	void Write(T data) noexcept;

	template<class T>
	void Write(Vector<T> data) noexcept;
	//===================================================

	//== Read ===========================================
	template<class T>
	void Read(T* data, const size_t& size) noexcept;

	template<class T>
	void Read(T* data) noexcept;

	template<class T>
	void Read(Vector<T>* data) noexcept;
	//===================================================

	/** file info */
	bool IsOpen() const noexcept;
	bool IsEof() const noexcept;
	OpenMode GetMode() const noexcept;

private:

	// * file data
	FILE* m_fp = nullptr;

	OpenMode m_openMode = OpenMode::Not_Mode;
};

template<class T>
FORCEINLINE void FileStream::Write(T* data, const size_t& size) noexcept
{
	if (m_openMode != OpenMode::Write_Mode)
		return;

	fwrite(reinterpret_cast<void*>(data), size, 1, m_fp);
}

template<class T>
FORCEINLINE void FileStream::Write(T data) noexcept
{
	Write(&data, sizeof(T));
}

template<>
FORCEINLINE void FileStream::Write(StringView data) noexcept
{
	if (m_openMode == OpenMode::Txt_Mode)
	{
		fputs(data.data(), m_fp);
	}
	else if (m_openMode == OpenMode::Write_Mode)
	{
		auto length = data.length();
		fwrite(reinterpret_cast<void*>(&length), sizeof(size_t), 1, m_fp);
		fwrite(reinterpret_cast<void*>(const_cast<char*>(data.data())), length, 1, m_fp);
	}
}

template<>
FORCEINLINE void FileStream::Write(String data) noexcept
{
	Write<StringView>(data);
}

template<class T>
FORCEINLINE void FileStream::Write(Vector<T> data) noexcept
{
	if (m_openMode != OpenMode::Write_Mode)
		return;

	auto size = data.size();
	fwrite(reinterpret_cast<void*>(&size), sizeof(size_t), 1, m_fp);
	fwrite(reinterpret_cast<void*>(data.data()), sizeof(T), size, m_fp);
}

template<>
FORCEINLINE void FileStream::Write<String>(Vector<String> data) noexcept
{
	if (m_openMode == OpenMode::Txt_Mode)
	{
		for (const auto& addText : data)
		{
			fputs(addText.data(), m_fp);
		}
	}
	else if (m_openMode == OpenMode::Write_Mode)
	{
		auto size = data.size();
		fwrite(reinterpret_cast<void*>(&size), sizeof(size_t), 1, m_fp);

		for (int i = 0; i < size; ++i)
		{
			auto str = data[i].data();
			auto length = data[i].length();

			fwrite(reinterpret_cast<void*>(&length), sizeof(size_t), 1, m_fp);
			fwrite(reinterpret_cast<void*>(const_cast<char*>(str)), length, 1, m_fp);
		}
	}
}

template<class T>
FORCEINLINE void FileStream::Read(T* data, const size_t& size) noexcept
{
	if (m_openMode != OpenMode::Read_Mode)
		return;

	fread(reinterpret_cast<void*>(data), size, 1, m_fp);
}

template<class T>
FORCEINLINE void FileStream::Read(T* data) noexcept
{
	Read(data, sizeof(T));
}

template<>
FORCEINLINE void FileStream::Read<String>(String* data) noexcept
{
	if (m_openMode == OpenMode::Txt_Mode)
	{
		// 終端チェック
		if (IsEof())
			return;

		char buf[256];
		fgets(buf, 256, m_fp);
		*data = buf;
	}
	else if (m_openMode == OpenMode::Read_Mode)
	{
		size_t length;
		fread(reinterpret_cast<void*>(&length), sizeof(size_t), 1, m_fp);

		data->resize(length);
		fread(reinterpret_cast<void*>(const_cast<char*>(data->data())), length, 1, m_fp);
	}
}

template<class T>
FORCEINLINE void FileStream::Read(Vector<T>* data) noexcept
{
	if (m_openMode != OpenMode::Read_Mode)
		return;

	size_t size;
	fread(reinterpret_cast<void*>(&size), sizeof(size_t), 1, m_fp);

	data->resize(size);
	fread(reinterpret_cast<void*>(data->data()), sizeof(T), size, m_fp);
}

template<>
FORCEINLINE void FileStream::Read<String>(Vector<String>* data) noexcept
{
	if (m_openMode == OpenMode::Txt_Mode)
	{
		/** ファイルの先頭から全てのデータを読み込む */
		if (fseek(m_fp, 0L, SEEK_SET) != 0)
			return;

		// 終端チェック
		while (!IsEof())
		{
			char buf[256];
			fgets(buf, 256, m_fp);
			data->push_back(std::move(buf));
		}
	}
	else if (m_openMode == OpenMode::Read_Mode)
	{
		size_t size;
		fread(reinterpret_cast<void*>(&size), sizeof(size_t), 1, m_fp);

		data->resize(size);
		for (int i = 0; i < size; ++i)
		{
			size_t length;
			fread(reinterpret_cast<void*>(&length), sizeof(size_t), 1, m_fp);

			data->at(i).resize(length);
			fread(reinterpret_cast<void*>(const_cast<char*>(data->at(i).data())), length, 1, m_fp);
		}
	}
}