/**
* @file	   ModelRenderDetails.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
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
		auto modelPath = model ? ConvertToJapanese(model->GetFilePath()) : String();

		ImGui::Text("Model"); ImGui::SameLine(offsetPos);

		constexpr auto itemWidth = 260;
		ImGui::PushItemWidth(itemWidth);
		ImGui::InputText("##", modelPath.data(), modelPath.size());
		ImGui::PopItemWidth();

		ImGui::SameLine(offsetPos + itemWidth);
		
		// リソースの検索を始める
		OpenResourceHelper();

		if (auto resourceData = ShowSearchResourceHelper<Model>())
		{
			auto model = LoadResource<Model>(resourceData);
			m_modelRender->SetModel(model);
		}

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

	if (ImGui::TreeNodeEx("Use Meshes"))
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

			constexpr auto itemWidth = 280;
			ImGui::PushItemWidth(itemWidth);
			ImGui::InputText("##", materialPath.data(), materialPath.size());
			ImGui::PopItemWidth();

			// ドラッグアンドドロップ有効指定
			const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
			
			ShowDragDropHelper<Material>(hoverd, 40, 255, 40);

			// ドラッグアンドドロップでの Material 切り替えのため
			if (ImGui::IsMouseReleased(0) && hoverd)
			{
				if (auto catchMaterial = CatchDragObject<Material>())
				{
					RegisterEditorCommand([mesh](auto data) { mesh->SetMaterial(data); }, catchMaterial, material);
				}
			}

			// 改行用
			ImGui::Text("");
		}
		ImGui::TreePop();
	}
}