/**
* @file    ResourceData.h
* @brief
*
* @date	   2022/08/07 2022�N�x����
*/
#pragma once


struct ResourcePath
{
	// �e���\�[�X���ʔԍ�
	uint32_t m_type;

	// ���\�[�X�f�[�^������t�@�C���p�X
	String m_path;

public:

	ResourcePath() = default;
	ResourcePath(uint32_t type, String path);
	ResourcePath(StringView type, String path);

	void Serialized(FileStream* file) const noexcept;
	void Deserialization(FileStream* file) noexcept;
};

struct ResourceData
{
	String m_assetName;

	String m_assetFullPath;

	ResourcePath m_resourcePath;

	// * �Q�Ɗ֌W�̃��\�[�X�f�[�^ (�������[�h�̂��߂Ɏg�p)
	Vector<ResourcePath> m_refResourcePaths;
	
	// * �Q�Ɗ֌W���畡���g�p�����Ȃǃ��[�h����Ă��鐔
	uint32_t m_numLoading = 0;

public:

	void Serialized(FileStream* file) const noexcept;
	void Deserialization(FileStream* file) noexcept;

	bool operator==(const ResourceData& data) noexcept;
	bool operator!=(const ResourceData& data) noexcept;
};