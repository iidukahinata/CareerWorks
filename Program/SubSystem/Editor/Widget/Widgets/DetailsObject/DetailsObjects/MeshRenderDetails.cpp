/**
* @file	   MeshRenderDetails.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "MeshRenderDetails.h"
#include "SubSystem/Scene/GameObject.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Scene/Component/IRenderObject.h"

MeshRenderDetails::MeshRenderDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_meshRender = dynamic_cast<IMeshRender*>(component);
	ASSERT(m_meshRender);

	m_gameObject = m_meshRender->GetOwner();
}

void MeshRenderDetails::Draw()
{
	constexpr int offsetPos = 130;

	if (ShowComponentHeader(m_meshRender))
	{
		auto meshRenderer = m_meshRender;
		auto mesh		  = meshRenderer->GetMesh();
		auto meshPath	  = mesh ? ConvertToJapanese(mesh->GetFilePath()) : "";

		ImGui::Text("Mesh"); ImGui::SameLine(offsetPos);

		constexpr auto itemWidth = 250;
		ImGui::PushItemWidth(itemWidth);
		ImGui::InputText("##", meshPath.data(), meshPath.size());
		ImGui::PopItemWidth();

		// ドラッグアンドドロップでの Texture 切り替え
		const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		if (ImGui::IsMouseReleased(0) && hoverd)
		{
			if (auto catchMesh = CatchDragObject<Mesh>())
			{
				RegisterEditorCommand([meshRenderer](auto data) { meshRenderer->SetMesh(data); }, catchMesh, meshRenderer->GetMesh());
			}
		}

		ShowDragDropHelper<Mesh>(hoverd, 34, 196, 255);

		ImGui::SameLine(offsetPos + itemWidth + 5);

		// リソースの検索を始める
		OpenResourceHelper("MeshRender");
		if (auto resourceData = ShowSearchResourceHelper<Mesh>())
		{
			if (auto catchMesh = LoadResource<Mesh>(resourceData))
			{
				RegisterEditorCommand([meshRenderer](auto data) { meshRenderer->SetMesh(data); }, catchMesh, meshRenderer->GetMesh());
			}
		}

		ImGui::Text("");

		if (mesh)
		{
			auto isUpdateMesh = false;

			auto material = mesh->GetMaterial();
			auto materialPath = ConvertToJapanese(material->GetFilePath());

			ImGui::Text("Material"); ImGui::SameLine(offsetPos);

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
					isUpdateMesh = true;
				}
			}

			ShowDragDropHelper<Material>(hoverd, 25, 255, 25);

			ImGui::SameLine(offsetPos + itemWidth + 5);

			// 検索での Material 切り替え
			OpenResourceHelper(materialPath);
			if (auto resourceData = ShowSearchResourceHelper<Material>())
			{
				if (auto catchMaterial = LoadResource<Material>(resourceData))
				{
					RegisterEditorCommand([mesh](auto data) { mesh->SetMaterial(data); }, catchMaterial, material);
					isUpdateMesh = true;
				}
			}

			if (isUpdateMesh)
			{
				mesh->Update();
			}
		}
	}
}