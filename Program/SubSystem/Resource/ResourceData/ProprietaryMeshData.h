/**
* @file    ProprietaryMeshData.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "ProprietaryData.h"
#include "SubSystem/Renderer/Geometry/Vertex.h"

struct aiMesh;
struct PMDMesh;
struct PMDMaterial;

struct ProprietaryMeshData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	// * PBR �p���_�f�[�^�ō쐬
	Vector<VertexBump3D> m_vertices;
	Vector<uint32_t> m_indices;

	// * ���b�V���Ŏg�p����}�e���A����
	String m_materialPath;

public:

	void Serialized(FileStream* file) const override;
	void Deserialization(FileStream* file) override;

	/** assimp �f�[�^����Ǝ��f�[�^�ɕϊ��A�e�N�X�`�������Ŏg�p */
	static ProprietaryMeshData ConvertProprietaryData(aiMesh* mesh) noexcept;
	/** pmd �f�[�^����Ǝ��f�[�^�ɕϊ��A�e�N�X�`�������Ŏg�p */
	static ProprietaryMeshData ConvertProprietaryData(PMDMesh* mesh) noexcept;

	/** �Ǝ��f�[�^�Ƃ��ĕۑ������o�͐�t�@�C���p�X�ɕϊ� */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};