/**
* @file    ProprietaryMeshData.cpp
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/


#include <assimp/scene.h>
#include "ProprietaryMeshData.h"
#include "../ResourceImporter/ModelImporter/PMDHelper.h"

void ProprietaryMeshData::Serialized(FileStream* file) const
{
	file->Write(m_vertices);
	file->Write(m_indices);
	file->Write(m_materialPath);
}

void ProprietaryMeshData::Deserialized(FileStream* file)
{
	file->Read(&m_vertices);
	file->Read(&m_indices);
	file->Read(&m_materialPath);
}

ProprietaryMeshData ProprietaryMeshData::ConvertProprietaryData(aiMesh* mesh) noexcept
{
	ProprietaryMeshData meshData = {};
	auto& vertices = meshData.m_vertices;
	auto& indices = meshData.m_indices;

	vertices.resize(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		// Mesh���璸�_���W���擾
		vertices[i].position.x = mesh->mVertices[i].x;
		vertices[i].position.y = mesh->mVertices[i].y;
		vertices[i].position.z = mesh->mVertices[i].z;

		// Mesh����@�����擾
		if (mesh->HasNormals())
		{
			vertices[i].normal.x = mesh->mNormals[i].x;
			vertices[i].normal.y = mesh->mNormals[i].y;
			vertices[i].normal.z = mesh->mNormals[i].z;
		}

		// Mesh����UV���W���擾
		if (mesh->HasTextureCoords(0))
		{
			vertices[i].tex.x = mesh->mTextureCoords[0][i].x;
			vertices[i].tex.y = mesh->mTextureCoords[0][i].y;
		}
	}

	Math::Vector3 cp[3][3];
	for (int face_i = 0; face_i < mesh->mNumFaces; ++face_i)
	{
		aiFace* face = &mesh->mFaces[face_i];
		for (int index_i = 0; index_i < face->mNumIndices; ++index_i)
		{
			const auto i = face->mIndices[index_i];
			indices.push_back(i);

			cp[index_i][0] = Math::Vector3(vertices[i].position.x, vertices[i].tex.x, vertices[i].tex.y);
			cp[index_i][1] = Math::Vector3(vertices[i].position.y, vertices[i].tex.x, vertices[i].tex.y);
			cp[index_i][2] = Math::Vector3(vertices[i].position.z, vertices[i].tex.x, vertices[i].tex.y);
		}

		// �ڃx�N�g�� & �]�x�N�g���v�Z
		Math::Vector3 tangent, binormal;
		for (int i = 0; i < 3; ++i)
		{
			const auto V1 = cp[1][i] - cp[0][i];
			const auto V2 = cp[2][i] - cp[1][i];
			const auto ABC = V1.Cross(V2);

			if (ABC.x == 0.f)
			{
				tangent[i] = 0.f;
				binormal[i] = 0.f;
			}
			else
			{
				tangent[i] = -ABC.y / ABC.x;
				binormal[i] = -ABC.z / ABC.x;
			}
		}

		// Normalize
		tangent.Normalize();
		binormal.Normalize();

		for (int index_i = 2; index_i >= 0; --index_i)
		{
			const auto i = face->mIndices[index_i];
			vertices[i].tangent = tangent;
			vertices[i].biNormal = binormal;
		}
	}

	return meshData;
}

ProprietaryMeshData ProprietaryMeshData::ConvertProprietaryData(PMDMesh* mesh) noexcept
{
	constexpr int faceNumIndices = 3;

	ProprietaryMeshData meshData = {};
	auto& vertices = meshData.m_vertices;
	auto& indices = meshData.m_indices;

	const auto numVertices = mesh->m_vertices.size();
	const auto numIndices = mesh->m_indices.size();

	vertices.resize(numVertices);
	for (int i = 0; i < numVertices; ++i)
	{
		// Mesh���璸�_���W���擾
		vertices[i].position = mesh->m_vertices[i].m_pos;

		// Mesh����@�����擾
		vertices[i].normal = mesh->m_vertices[i].m_normal;

		// Mesh����UV���W���擾
		vertices[i].tex = mesh->m_vertices[i].m_uv;
	}

	Math::Vector3 cp[3][3];
	for (int face_i = 0; face_i < numIndices; face_i += faceNumIndices)
	{
		// �ڃx�N�g�� & �]�x�N�g���v�Z�Ɏg�p����p�����[�^�[�擾
		for (int index_i = 0; index_i < faceNumIndices; ++index_i)
		{
			const auto i = mesh->m_indices[index_i + face_i];
			indices.push_back(static_cast<uint32_t>(i));

			cp[index_i][0] = Math::Vector3(vertices[i].position.x, vertices[i].tex.x, vertices[i].tex.y);
			cp[index_i][1] = Math::Vector3(vertices[i].position.y, vertices[i].tex.x, vertices[i].tex.y);
			cp[index_i][2] = Math::Vector3(vertices[i].position.z, vertices[i].tex.x, vertices[i].tex.y);
		}

		// �ڃx�N�g�� & �]�x�N�g���v�Z
		Math::Vector3 tangent, binormal;
		for (int i = 0; i < 3; ++i)
		{
			const auto V1 = cp[1][i] - cp[0][i];
			const auto V2 = cp[2][i] - cp[1][i];
			const auto ABC = V1.Cross(V2);

			if (ABC.x == 0.f)
			{
				tangent[i] = 0.f;
				binormal[i] = 0.f;
			}
			else
			{
				tangent[i] = -ABC.y / ABC.x;
				binormal[i] = -ABC.z / ABC.x;
			}
		}

		// Normalize
		tangent.Normalize();
		binormal.Normalize();

		const auto endIndices_i = (face_i + faceNumIndices);
		for (int index_i = face_i; index_i < endIndices_i; ++index_i)
		{
			const auto i = mesh->m_indices[index_i];
			vertices[i].tangent = tangent;
			vertices[i].biNormal = binormal;
		}
	}

	return meshData;
}

String ProprietaryMeshData::ConvertProprietaryPath(StringView filePath) noexcept
{
	// �t�@�C���g���q��Ǝ��t�@�C���p�ɕύX���邽�ߏ���
	filePath = filePath.substr(0, filePath.find("."));

	return (MESH_DIRECTORY + FileSystem::GetFilePath(filePath) + MESH_EXTENSION);
}