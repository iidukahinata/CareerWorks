/**
* @file    Meshcpp
* @brief
*
* @date	   2022/12/01 2022年度初版
*/


#include "Mesh.h"
#include "Material.h"
#include "SubSystem/Resource/ResourceManager.h"

bool Mesh::Load(StringView path)
{
	if (!m_meshData.LoadFromFile(path))
	{
		return false;
	}

	if (!m_vertexBuffer.Create(m_meshData.m_vertices))
	{
		return false;
	}

	if (!m_indexBuffer.Create(m_meshData.m_indices))
	{
		return false;
	}

	if (!m_meshData.m_materialPath.empty())
	{
		auto& materialPath = m_meshData.m_materialPath;
		auto resourceData = m_resourceManager->GetResourceData(materialPath);
		auto material = m_resourceManager->GetResource(resourceData);

		SetMaterial(dynamic_cast<Material*>(material));
	}

	return true;
}

void Mesh::Update()
{
	UpdateProprietaryDataFile();

	UpdateResourceDataFile();
}

void Mesh::SetMaterial(Material* material) noexcept
{
	m_material = material;
}

Material* Mesh::GetMaterial() const noexcept
{
	return m_material;
}

D3D12VertexBuffer* Mesh::GetVertexBuffer() noexcept
{
	return &m_vertexBuffer;
}

D3D12IndexBuffer* Mesh::GetIndexBuffer() noexcept
{
	return &m_indexBuffer;
}

uint32_t Mesh::GetIndexNum() const noexcept
{
	return m_meshData.m_indices.size();
}

void Mesh::UpdateProprietaryDataFile() noexcept
{
	// 元データの消去
	m_meshData.m_materialPath = String();

	// 次回呼び出し時適用するパラメーターパスを登録
	if (m_material)
	{
		m_meshData.m_materialPath = m_material->GetFilePath();
	}

	m_meshData.SaveToFile(m_filePath);
}

void Mesh::UpdateResourceDataFile() noexcept
{
	auto resourceData = GetResourceData();
	auto& refResourcePaths = resourceData->m_refResourcePaths;

	// 元データの消去
	refResourcePaths.clear();
	refResourcePaths.shrink_to_fit();

	if (m_material)
	{
		auto& materialPath = m_material->GetFilePath();
		refResourcePaths.emplace_back(ResourcePath(Material::TypeData.Hash, materialPath));
	}

	m_resourceManager->UpdateResourceData(resourceData);
}