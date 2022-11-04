/**
* @file    MeshRender.cpp
* @brief
*
* @date	   2022/10/28 2022年度初版
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"

void ModelRender::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	if (m_model)
	{
		file->Write(m_model->GetFilePath());
	}
	else
	{
		file->Write(String());
	}
}

void ModelRender::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	String modelPath;
	file->Read(&modelPath);

	if (modelPath.empty())
	{
		return;
	}

	if (auto resourceManager = GetContext()->GetSubsystem<ResourceManager>())
	{
		if (auto resourceData = resourceManager->GetResourceData(modelPath))
		{
			auto resource = resourceManager->GetResource(resourceData);
			SetModel(resource);
		}
	}
}

void ModelRender::SetModel(IResource* resource) noexcept
{
	if (auto model = dynamic_cast<Model*>(resource))
	{
		SetModel(model);
	}
}

void ModelRender::SetModel(Model* model) noexcept
{
	m_model = model;
}

Model* ModelRender::GetModel() const noexcept
{
	return m_model;
}

void ModelRender::PreRender()
{
	if (!m_model)
	{
		return;
	}

	// 描画時使用する MatrixBuffer の更新
	{
		auto&& handle = m_constantBufferMatrix.GetCPUData();
		auto&& transform = GetTransform().GetWorldMatrix().ToMatrixXM();

		m_renderer->GetTransformCBuffer()->Bind(handle, transform);
		m_constantBufferMatrix.VSSet(0);
	}

	for (auto mesh : m_model->GetAllMeshes())
	{
		if (!mesh->GetMaterial())
			continue;

		mesh->PreRender();
	}
}

void ModelRender::Render()
{
	if (!m_model)
	{
		return;
	}

	// PreRender で更新済み
	m_constantBufferMatrix.VSSet(0);

	for (const auto& meshes = m_model->GetAllMeshes(); auto mesh : meshes)
	{
		if (!mesh->GetMaterial())
			continue;

		mesh->Render();
	}
}