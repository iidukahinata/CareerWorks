/**
* @file	   DetailsObject.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
*/


#include "DetailsObject.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"

DetailsObject::DetailsObject(DetailsWidget* detailsWidget) :
	m_detailsWidget(detailsWidget)
{

}

void DetailsObject::ShowDragDropHelper(uint32_t selctType, bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();
	min.x -= 3; min.y -= 3;
	max.x += 3; max.y += 3;

	if (DragDrop::Get().HasResource())
	{
		constexpr int select = 255;
		constexpr int noSelect = 200;
		int thickness = 3;

		const auto resrouceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
		const auto type = resrouceData->m_resourcePath.m_type;

		if (type == selctType)
		{
			if (hovered)
			{
				// pop up 的な動きをさせる
				min.x -= 2; min.y -= 2;
				max.x += 2; max.y += 2;
				thickness = 4;
			}

			// 指定型のみ受付する
			auto alpha = hovered ? select : noSelect;
			draw_list->AddRect(min, max, IM_COL32(r, g, b, alpha), 0, 0, thickness);
		}
		else
		{
			// 赤い禁止するような UI 表示
			draw_list->AddLine(min, max, IM_COL32(200, 0, 0, 180), thickness);
			draw_list->AddRect(min, max, IM_COL32(200, 0, 0, 210), 0, 0, 1);
		}
	}
	else
	{
		// 細目の白いライン表示
		draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 120), 0, 0, 1);
	}
}

IResource* DetailsObject::CatchDragResourceObject(uint32_t selectType) const noexcept
{
	if (!DragDrop::Get().HasResource())
	{
		return nullptr;
	}

	const auto resourceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
	const auto type = resourceData->m_resourcePath.m_type;

	if (type != selectType)
	{
		return nullptr;
	}

	return LoadResource(resourceData);
}

ResourceData* DetailsObject::CatchDragResourceData() const noexcept
{
	if (!DragDrop::Get().HasResource())
	{
		return nullptr;
	}

	return std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
}

GameObject* DetailsObject::CatchDragGameObject() const noexcept
{
	if (!DragDrop::Get().HasGameObject())
	{
		return nullptr;
	}

	return std::any_cast<GameObject*>(DragDrop::Get().GetDragObject());
}

bool DetailsObject::OpenResourceHelper(UINT index /* = 0 */) noexcept
{
	if(ImGui::Button((String("*##") + std::to_string(index)).c_str(), ImVec2(20, 20)))
	{
		ImGui::OpenPopup("SearchResourceHelper");
		return true;
	}
	return false;
}

ResourceData* DetailsObject::ShowSearchResourceHelper(uint32_t selectType) noexcept
{
	ResourceData* resourceData = nullptr;

	if (ImGui::BeginPopup("SearchResourceHelper"))
	{
		// 文字列検索用 Filter 定義
		m_searchResourceFilter.Draw("Search", 220.0f);

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(290, 300), true, ImGuiWindowFlags_NoScrollbar);

		constexpr auto columns = 2;
		ImGui::Columns(columns, nullptr, false);

		const auto&  resourceManager = m_detailsWidget->GetResourceManager();
		const auto&  resourceDataMap = resourceManager->GetResourceDataListByType(selectType);

		for (const auto& resourceInfo : resourceDataMap)
		{
			const auto& assetName = resourceInfo.first;
			const auto& assetData = resourceInfo.second;

			// 指定文字列を含む Resource のみ列挙
			if (!m_searchResourceFilter.PassFilter(assetName.c_str()))
			{
				continue;
			}

			auto&& label = ConvertToJapanese(assetName);
			if (ImGui::Button(label.c_str(), ImVec2(130, 120)))
			{
				resourceData = resourceManager->GetResourceData(selectType, assetName);
				ImGui::CloseCurrentPopup();
				break;
			}
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::EndPopup();
	}

	return resourceData;
}

IResource* DetailsObject::LoadResource(ResourceData* resourceData) const noexcept
{
	auto resourceHandle = m_detailsWidget->GetResourceManager()->Load(resourceData);
	resourceHandle->WaitForLoadComplete();

	return resourceHandle->GetResource();
}