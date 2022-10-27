/**
* @file	   ModelRenderDetails.cpp
* @brief
*
* @date	   2022/10/23 2022年度初版
*/

#include "ModelRenderDetails.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

ModelRenderDetails::ModelRenderDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_modelRender = dynamic_cast<ModelRender*>(component);
	ASSERT(m_modelRender);
}

void ModelRenderDetails::Draw()
{
	constexpr int offsetPos = 130;

	if (ShowComponentHeader(m_modelRender))
	{
		auto modelRenderer = m_modelRender;
		auto modelPath = modelRenderer->GetModel() ? ConvertToJapanese(modelRenderer->GetModel()->GetFilePath()) : String();

		ImGui::Text("Model"); ImGui::SameLine(offsetPos);

		constexpr auto itemWidth = 250;
		ImGui::PushItemWidth(itemWidth);
		ImGui::InputText("##", modelPath.data(), modelPath.size());
		ImGui::PopItemWidth();

		// ドラッグアンドドロップでの Texture 切り替え
		const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		if (ImGui::IsMouseReleased(0) && hoverd)
		{
			if (auto catchModel = CatchDragObject<Model>())
			{
				RegisterEditorCommand([modelRenderer](auto data) { modelRenderer->SetModel(data); }, catchModel, modelRenderer->GetModel());
			}
		}

		ShowDragDropHelper<Model>(hoverd, 22, 120, 255);

		ImGui::SameLine(offsetPos + itemWidth + 5);
		
		// リソースの検索を始める
		OpenResourceHelper();
		if (auto resourceData = ShowSearchResourceHelper<Model>())
		{
			auto catchModel = LoadResource<Model>(resourceData);
			RegisterEditorCommand([modelRenderer](auto data) { modelRenderer->SetModel(data); }, catchModel, modelRenderer->GetModel());
		}

		if (auto model = modelRenderer->GetModel())
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

			constexpr auto itemWidth = 250;
			ImGui::PushItemWidth(itemWidth);
			ImGui::InputText("##", materialPath.data(), materialPath.size());
			ImGui::PopItemWidth();

			// ドラッグアンドドロップでの Material 切り替え
			const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
			if (ImGui::IsMouseReleased(0) && hoverd)
			{
				if (auto catchMaterial = CatchDragObject<Material>())
				{
					RegisterEditorCommand([mesh](auto data) { mesh->SetMaterial(data); }, catchMaterial, material);
				}
			}
			
			ShowDragDropHelper<Material>(hoverd, 25, 255, 25);

			ImGui::SameLine(offsetPos + itemWidth + 5);

			static auto selectMat = 0;
			if (OpenResourceHelper(i))
			{
				// Resource Helper が複数個呼び出される可能性があるため ID で選択中かを判別
				selectMat = i;
			}

			// 検索での Material 切り替え
			if (selectMat == i)
			{
				if (auto resourceData = ShowSearchResourceHelper<Material>())
				{
					if (auto catchMaterial = CatchDragObject<Material>())
					{
						RegisterEditorCommand([mesh](auto data) { mesh->SetMaterial(data); }, catchMaterial, material);
					}
				}
			}

			// 改行用
			ImGui::Text("");
		}
		ImGui::TreePop();
	}
}