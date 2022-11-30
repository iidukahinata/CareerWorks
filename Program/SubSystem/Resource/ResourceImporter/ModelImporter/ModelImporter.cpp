/**
* @file		ModelImporter.cpp
* @brief
*
* @date		2022/08/07 2022�N�x����
*/


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include "PMDHelper.h"
#include "ModelImporter.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Renderer/Geometry/Vertex.h"
#include "../TextureImporter/TextureImporter.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/3DModel/Texture.h"
#include "SubSystem/Resource/ResourceData/ProprietaryTextureData.h"

ModelImporter::ModelImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{

}

bool ModelImporter::CreateModelData(StringView filePath) noexcept
{
	// �V�K���\�[�X�������Ɏg�p�����ƃf�B���N�g�����擾
	m_currentDirectory = filePath.substr(0, filePath.find_last_of("\\/") + 1);

	if (!ImportFile(filePath))
	{
		return false;
	}

	// PMD�g���q�̎��� NULL
	if (m_aiScene)
	{
		ProcessNode(m_aiScene->mRootNode);
	}

	// ���f���f�[�^�쐬
	auto modelPath = ProprietaryModelData::ConvertProprietaryPath(filePath);
	{
		ProprietaryModelData modelData = {};
		modelData.m_meshPaths = m_meshPaths;

		modelData.SaveToFile(modelPath);
	}

	CreateResourceData(modelPath);

	Release();

	return true;
}

void ModelImporter::CreateResourceData(StringView filePath) noexcept
{
	auto modelData = m_resourceManager->CreateResourceData<Model>(filePath);

	for (const auto& meshPath : m_meshPaths)
	{
		const auto& materialPath = m_materialPaths[meshPath];
		const auto& texturePaths = m_texturePaths[materialPath];

		// ���f���Q�Ɗ֌W��o�^
		auto meshData = m_resourceManager->CreateResourceData<Mesh>(meshPath);
		modelData->m_refResourcePaths.emplace_back(meshData->m_resourcePath);

		// ���b�V���Q�Ɗ֌W��o�^
		auto materialData = m_resourceManager->CreateResourceData<Material>(materialPath);
		meshData->m_refResourcePaths.emplace_back(materialData->m_resourcePath);

		// �}�e���A���Q�Ɗ֌W��o�^
		for (const auto& texturePath : texturePaths)
		{
			auto textureData = m_resourceManager->CreateResourceData<Texture>(texturePath);
			materialData->m_refResourcePaths.emplace_back(textureData->m_resourcePath);
		}

		m_resourceManager->UpdateResourceData(materialData);
		m_resourceManager->UpdateResourceData(meshData);
	}

	m_resourceManager->UpdateResourceData(modelData);
}

void ModelImporter::Release() noexcept
{
	if (m_aiScene)
	{
		aiReleaseImport(m_aiScene);
		m_aiScene = nullptr;
	}
}

void ModelImporter::ProcessNode(aiNode* node)
{
	ASSERT(node);
	
	// 2022/05/01���_�ł̓A�j���[�V�������͓ǂݍ��܂Ȃ�
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		auto meshPath = LoadMesh(m_aiScene->mMeshes[node->mMeshes[i]]);

		m_meshPaths.emplace_back(meshPath);
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i]);
	}
}

String ModelImporter::LoadMesh(aiMesh* mesh)
{
	ASSERT(mesh);

	auto path = m_currentDirectory + FileSystem::GetFilePath(mesh->mName.C_Str());

	// �e�f�[�^��Ǝ����f���Ƃ��ďo�͂��邽�ߕϊ�
	auto meshPath = ProprietaryMeshData::ConvertProprietaryPath(path);
	auto meshData = ProprietaryMeshData::ConvertProprietaryData(mesh);

	// ���b�V���Ŏg�p����Ă���}�e���A�������
	if (mesh->mMaterialIndex >= 0)
	{
		auto materialPath = LoadMaterial(m_aiScene->mMaterials[mesh->mMaterialIndex]);

		meshData.m_materialPath = materialPath;
		m_materialPaths.emplace(meshPath, materialPath);
	}

	meshData.SaveToFile(meshPath);

	return meshPath;
}

String ModelImporter::LoadMaterial(aiMaterial* material)
{
	ASSERT(material);

	auto path = m_currentDirectory + FileSystem::GetFilePath(material->GetName().C_Str());

	// �e�f�[�^��Ǝ����f���Ƃ��ďo�͂��邽�ߕϊ�
	auto materialPath = ProprietaryMaterialData::ConvertProprietaryPath(path);
	auto materialData = ProprietaryMaterialData::ConvertProprietaryData(material);

	materialData.SaveToFile(materialPath);

	return materialPath;
}

Vector<String> ModelImporter::LoadTextures(aiMaterial* material, bool isCreate)
{
	ASSERT(material);

	auto textureImporter = m_resourceManager->GetTextureImporter();

	// �}�e���A������e�N�X�`�������擾��(��{��1��)���[�v������
	const auto numTexture = material->GetTextureCount(aiTextureType_DIFFUSE);

	Vector<String> texturePaths(numTexture);

	for (int i = 0; i < numTexture; ++i)
	{
		aiString str;

		// �}�e���A�����炉�Ԗڂ̃e�N�X�`���t�@�C�������擾����
		material->GetTexture(aiTextureType_DIFFUSE, i, &str);

		String path(str.C_Str());

		// �e�N�X�`�����̃f�B���N�g���������o����J�����g�f�B���N�g���ɕύX
		path = path.substr(path.find_last_of("\\/"), path.length() - 1);
		auto filePath = FileSystem::Canonical(String("./") + path.data());

		if (isCreate)
		{
			textureImporter->CreateTextureData(filePath);
		}

		texturePaths[i] = ProprietaryTextureData::ConvertProprietaryPath(filePath);
	}

	return texturePaths;
}

String ModelImporter::LoadMesh(PMDMesh* mesh, StringView path)
{
	// �e�f�[�^��Ǝ����f���Ƃ��ďo�͂��邽�ߕϊ�
	// Path �� asset �f�[�^�쐬���Ƀ}�e���A���Ɠ����ɂȂ�Ȃ��悤�ɁA�������ǉ�
	auto meshPath = ProprietaryMeshData::ConvertProprietaryPath(path + "Mesh");
	auto meshData = ProprietaryMeshData::ConvertProprietaryData(mesh);

	// ���b�V���Ŏg�p����Ă���}�e���A�������
	auto materialPath = LoadMaterial(&mesh->m_material, path);

	meshData.m_materialPath = materialPath;
	m_materialPaths.emplace(meshPath, materialPath);

	meshData.SaveToFile(meshPath);

	return meshPath;
}

String ModelImporter::LoadMaterial(PMDMaterial* material, StringView path)
{
	// �e�f�[�^��Ǝ����f���Ƃ��ďo�͂��邽�ߕϊ�
	// Path �� asset �f�[�^�쐬���Ƀ��b�V���Ɠ����ɂȂ�Ȃ��悤�ɁA�������ǉ�
	auto materialPath = ProprietaryMaterialData::ConvertProprietaryPath(path + "Material");
	auto materialData = ProprietaryMaterialData::ConvertProprietaryData(material);

	materialData.SaveToFile(materialPath);

	return materialPath;
}

String ModelImporter::LoadTexture(PMDMaterial* material, bool isCreate)
{
	ASSERT(material);

	auto textureImporter = m_resourceManager->GetTextureImporter();

	String path = material->m_texFilePath;

	// �e�N�X�`�����̃f�B���N�g���������o����J�����g�f�B���N�g���ɕύX
	path = path.substr(path.find_last_of("\\/"), path.length() - 1);
	auto filePath = FileSystem::Canonical(String("./") + path.data());

	// �}�e���A������擾�����e�N�X�`�������[�h
	if (isCreate)
	{
		textureImporter->CreateTextureData(filePath);
	}

	auto texturePath = ProprietaryTextureData::ConvertProprietaryPath(filePath);
	return texturePath;
}

bool ModelImporter::ImportFile(StringView filePath) noexcept
{
	if (GetExt(filePath) == "pmd")
	{
		return LoadPMDFile(filePath);
	}
	else
	{
		m_aiScene = aiImportFile(filePath.data(), aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
		return (!!m_aiScene);
	}
}

bool ModelImporter::LoadPMDFile(StringView filePath) noexcept
{
	FileStream file;
	if (!file.Load(filePath, OpenMode::Read_Mode))
	{
		return false;
	}

	char signature[3];
	file.Read(signature, sizeof(signature));

	PMDHeader pmdheader = {};
	file.Read(&pmdheader, sizeof(PMDHeader));

	uint32_t verticesNum;
	file.Read(&verticesNum, sizeof(uint32_t));

	Vector<PMDVertex> vertices(verticesNum);
	file.Read(vertices.data(), vertices.size() * sizeof(PMDVertex));

	uint32_t indicesNum;
	file.Read(&indicesNum, sizeof(uint32_t));

	Vector<uint16_t> indices(indicesNum);
	file.Read(indices.data(), indices.size() * sizeof(uint16_t));

	uint32_t materialNum;
	file.Read(&materialNum, sizeof(uint32_t));

	Vector<PMDMaterial> materials(materialNum);
	file.Read(materials.data(), materials.size() * sizeof(PMDMaterial));

	PMDMesh mesh = {};
	mesh.m_vertices = std::move(vertices);

	// PMD�}�e���A���� Index �������� Mesh ���\���B
	uint32_t indicesOffset = 0;
	for (int i = 0; i < materialNum; ++i)
	{
		const auto& material = materials[i];

		mesh.m_material = material;

		// Mesh ���\������ Index ���݂̂��R�s�[
		{
			auto beginIndices = indices.begin() + indicesOffset;
			auto endIndices = indices.begin() + indicesOffset + material.m_indicesNum;

			mesh.m_indices.resize(material.m_indicesNum);
			std::copy(beginIndices, endIndices, mesh.m_indices.begin());

			indicesOffset += material.m_indicesNum;
		}

		// �������O�� Mesh �������悤�� ID �𖼑O�ɒǉ�
		const auto path = filePath.substr(0, filePath.find(".")) + "_" + std::to_string(i) + "_";

		auto meshPath = LoadMesh(&mesh, path);

		m_meshPaths.emplace_back(meshPath);
	}

	return true;
}