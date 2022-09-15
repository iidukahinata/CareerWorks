/**
* @file    Model.cpp
* @brief
*
* @date	   2022/09/04 2022年度初版
*/


#include "Model.h"
#include "Mesh.h"
#include "SubSystem/Resource/ResourceManager.h"

bool Model::Load(StringView path)
{
	if (!m_modelData.LoadFromFile(path))
	{
		return false;
	}

	for (const auto& meshPath : m_modelData.m_meshPaths)
	{
		auto resourceData = m_resourceManager->GetResourceData(meshPath);
		auto resource = m_resourceManager->GetResource(resourceData);

		AddMesh(dynamic_cast<Mesh*>(resource));
	}

	return true;
}

void Model::Update()
{
	UpdateProprietaryDataFile();

	UpdateResourceDataFile();
}

void Model::AddMesh(Mesh* mesh) noexcept
{
	m_meshes.emplace_back(mesh);
}

Mesh* Model::GetMesh(uint32_t index) noexcept
{
	if (m_meshes.size() > index)
	{
		return m_meshes[index];
	}

	return nullptr;
}

const Vector<Mesh*>& Model::GetAllMeshes() noexcept
{
	return m_meshes;
}

void Model::UpdateProprietaryDataFile() noexcept
{
	m_modelData.m_meshPaths.clear();

	for (auto mesh : m_meshes)
	{
		const auto& meshPath = mesh->GetFilePath();
		m_modelData.m_meshPaths.emplace_back(meshPath);
	}

	m_modelData.SaveToFile(m_filePath);
}

void Model::UpdateResourceDataFile() noexcept
{
	auto resourceData = GetResourceData();
	auto& refResourcePaths = resourceData->m_refResourcePaths;

	// 元データの消去
	refResourcePaths.clear();
	refResourcePaths.shrink_to_fit();

	for (const auto mesh : m_meshes)
	{
		const auto& meshPath = mesh->GetFilePath();
		refResourcePaths.emplace_back(ResourcePath(Mesh::TypeData.Hash, meshPath));
	}

	m_resourceManager->UpdateResourceData(resourceData);
}