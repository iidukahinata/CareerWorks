/**
* @file		ModelImporter.cpp
* @brief
*
* @date		2022/08/07 2022年度初版
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
	// 新規リソース生成時に使用する作業ディレクトリを取得
	m_currentDirectory = filePath.substr(0, filePath.find_last_of("\\/") + 1);

	if (!ImportFile(filePath))
	{
		return false;
	}

	// PMD拡張子の時は NULL
	if (m_aiScene)
	{
		ProcessNode(m_aiScene->mRootNode);
	}

	// モデルデータ作成
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

		// モデル参照関係を登録
		auto meshData = m_resourceManager->CreateResourceData<Mesh>(meshPath);
		modelData->m_refResourcePaths.emplace_back(meshData->m_resourcePath);

		// メッシュ参照関係を登録
		auto materialData = m_resourceManager->CreateResourceData<Material>(materialPath);
		meshData->m_refResourcePaths.emplace_back(materialData->m_resourcePath);

		// マテリアル参照関係を登録
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
	
	// 2022/05/01時点ではアニメーション等は読み込まない
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

	// 各データを独自モデルとして出力するため変換
	auto meshPath = ProprietaryMeshData::ConvertProprietaryPath(path);
	auto meshData = ProprietaryMeshData::ConvertProprietaryData(mesh);

	// メッシュで使用されているマテリアルを解析
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

	// 各データを独自モデルとして出力するため変換
	auto materialPath = ProprietaryMaterialData::ConvertProprietaryPath(path);
	auto materialData = ProprietaryMaterialData::ConvertProprietaryData(material);

	materialData.SaveToFile(materialPath);

	return materialPath;
}

Vector<String> ModelImporter::LoadTextures(aiMaterial* material, bool isCreate)
{
	ASSERT(material);

	auto textureImporter = m_resourceManager->GetTextureImporter();

	// マテリアルからテクスチャ個数を取得し(基本は1個)ループさせる
	const auto numTexture = material->GetTextureCount(aiTextureType_DIFFUSE);

	Vector<String> texturePaths(numTexture);

	for (int i = 0; i < numTexture; ++i)
	{
		aiString str;

		// マテリアルからｉ番目のテクスチャファイル名を取得する
		material->GetTexture(aiTextureType_DIFFUSE, i, &str);

		String path(str.C_Str());

		// テクスチャ名のディレクトリを処理出来るカレントディレクトリに変更
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
	// 各データを独自モデルとして出力するため変換
	// Path は asset データ作成時にマテリアルと同じにならないように、文字列を追加
	auto meshPath = ProprietaryMeshData::ConvertProprietaryPath(path + "Mesh");
	auto meshData = ProprietaryMeshData::ConvertProprietaryData(mesh);

	// メッシュで使用されているマテリアルを解析
	auto materialPath = LoadMaterial(&mesh->m_material, path);

	meshData.m_materialPath = materialPath;
	m_materialPaths.emplace(meshPath, materialPath);

	meshData.SaveToFile(meshPath);

	return meshPath;
}

String ModelImporter::LoadMaterial(PMDMaterial* material, StringView path)
{
	// 各データを独自モデルとして出力するため変換
	// Path は asset データ作成時にメッシュと同じにならないように、文字列を追加
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

	// テクスチャ名のディレクトリを処理出来るカレントディレクトリに変更
	path = path.substr(path.find_last_of("\\/"), path.length() - 1);
	auto filePath = FileSystem::Canonical(String("./") + path.data());

	// マテリアルから取得したテクスチャをロード
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

	// PMDマテリアルの Index 情報を元に Mesh を構成。
	uint32_t indicesOffset = 0;
	for (int i = 0; i < materialNum; ++i)
	{
		const auto& material = materials[i];

		mesh.m_material = material;

		// Mesh を構成する Index 情報のみをコピー
		{
			auto beginIndices = indices.begin() + indicesOffset;
			auto endIndices = indices.begin() + indicesOffset + material.m_indicesNum;

			mesh.m_indices.resize(material.m_indicesNum);
			std::copy(beginIndices, endIndices, mesh.m_indices.begin());

			indicesOffset += material.m_indicesNum;
		}

		// 同じ名前の Mesh が無いように ID を名前に追加
		const auto path = filePath.substr(0, filePath.find(".")) + "_" + std::to_string(i) + "_";

		auto meshPath = LoadMesh(&mesh, path);

		m_meshPaths.emplace_back(meshPath);
	}

	return true;
}