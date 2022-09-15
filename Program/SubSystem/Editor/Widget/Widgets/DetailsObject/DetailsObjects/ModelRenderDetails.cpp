/**
* @file	   ModelRenderDetails.cpp
* @brief
*
* @date	   2022/09/13 2022�N�x����
*/

#include "ModelRenderDetails.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

ModelRenderDetails::ModelRenderDetails(DetailsWidget* detailsWidget, IComponent* component) :
	DetailsObject(detailsWidget)
{
	m_modelRender = dynamic_cast<ModelRender*>(component);
	ASSERT(m_modelRender);
}

void ModelRenderDetails::Draw()
{
	constexpr int offsetPos = 130;
	const auto& name = m_modelRender->TypeData.Name;

	if (ImGui::CollapsingHeader(name.data(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto model = m_modelRender->GetModel();
		auto modelPath = model ? ConvertToJapanese(model->GetFilePath()) : nullptr;

		ImGui::Text("Model"); ImGui::SameLine(offsetPos);
		ImGui::Text(modelPath.c_str());

		if (model)
		{
			ShowUseMeshes(model);
			ShowUseMaterial(model);
		}
	}
}

void ModelRenderDetails::ShowUseMeshes(Model* model) const noexcept
{
	constexpr int offsetPos = 130;

	if (ImGui::TreeNodeEx("Use Meshes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(710, 270), true, ImGuiWindowFlags_NoScrollbar);

		auto meshes = model->GetAllMeshes();
		for (int i = 0; i < meshes.size(); ++i)
		{
			auto mesh = meshes[i];
			auto meshPath = ConvertToJapanese(mesh->GetFilePath());

			ImGui::Text(("Mesh_" + std::to_string(i)).c_str()); ImGui::SameLine(offsetPos);
			ImGui::Text(meshPath.c_str());
		}

		ImGui::EndChild();
		ImGui::TreePop();
	}
}

void ModelRenderDetails::ShowUseMaterial(Model* model) noexcept
{
	constexpr int offsetPos = 130;

	if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto meshes = model->GetAllMeshes();
		for (int i = 0; i < meshes.size(); ++i)
		{
			auto mesh = meshes[i];
			auto material = mesh->GetMaterial();

			auto materialPath = ConvertToJapanese(material->GetFilePath());

			ImGui::Text(("Material_" + std::to_string(i)).c_str()); ImGui::SameLine(offsetPos);
			ImGui::Text(materialPath.c_str());

			// �h���b�O�A���h�h���b�v�L���w��
			const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
			
			ShowDragDropHelper<Material>(hoverd, 40, 255, 40);
			
			// �h���b�O�A���h�h���b�v�ł� Material �؂�ւ��̂���
			if (ImGui::IsMouseReleased(0) && hoverd)
			{
				if (auto catchMaterial = CatchDragObject<Material>())
				{
					mesh->SetMaterial(catchMaterial);
				}
			}

			// ���s�p
			ImGui::Text("");
		}
		ImGui::TreePop();
	}
}