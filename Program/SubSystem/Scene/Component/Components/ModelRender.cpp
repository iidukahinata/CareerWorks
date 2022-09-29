/**
* @file    MeshRender.cpp
* @brief
*
* @date	   2022/09/09 2022年度初版
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

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

void ModelRender::Deserialization(FileStream* file)
{
	IComponent::Deserialization(file);

	String modelPath;
	file->Read(&modelPath);

	if (modelPath.empty())
	{
		return;
	}

	if (auto resourceManager = GetContext()->GetSubsystem<ResourceManager>())
	{
		auto resourceData = resourceManager->GetResourceData(modelPath);
		auto resource = resourceManager->GetResource(resourceData);

		SetModel(resource);
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

void ModelRender::Render() const
{
	if (m_model)
	{
		// 描画時使用する World行列
		auto&& transform = GetTransform().GetWorldMatrix();
		m_renderer->GetTransformCBuffer()->SetWorld(transform.ToMatrixXM());
		m_renderer->GetTransformCBuffer()->Bind();

		for (const auto& meshes = m_model->GetAllMeshes(); auto mesh : meshes)
		{
			mesh->Render();
		}
	}
}