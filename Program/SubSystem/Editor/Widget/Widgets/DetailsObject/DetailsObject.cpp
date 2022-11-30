/**
* @file	   DetailsObject.cpp
* @brief
*
* @date	   2022/10/27 2022年度初版
*/


#include "DetailsObject.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"

DetailsObject::DetailsObject(DetailsWidget* detailsWidget) :
	m_detailsWidget(detailsWidget)
{

}

DetailsObject::~DetailsObject()
{

}

void DetailsObject::ShowDragDropHelper(uint32_t selctType, bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();

	// 少し大きめの枠にする
	min.x -= 3; min.y -= 3;
	max.x += 3; max.y += 3;

	if (DragDrop::Get().HasResource())
	{
		constexpr int selectAlpha = 255;
		constexpr int noSelectAlpha = 200;
		int thickness = 3;

		const auto resrouceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
		const auto type = resrouceData->m_resourcePath.m_type;

		// 同じリソースタイプのみ受付
		if (type == selctType)
		{
			if (hovered)
			{
				// pop up 的な動きをさせる
				min.x -= 2; min.y -= 2;
				max.x += 2; max.y += 2;
				thickness = 4;
			}

			auto alpha = hovered ? selectAlpha : noSelectAlpha;
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

ResourceData* DetailsObject::ShowSearchResourceHelper(uint32_t selectType) noexcept
{
	ResourceData* resourceData = nullptr;

	if (ImGui::BeginPopup(m_searchResourceLavel.c_str()))
	{
		// 文字列検索用 Filter 定義
		m_searchResourceFilter.Draw("Search", 220.0f);

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(290, 300), true, ImGuiWindowFlags_NoScrollbar);

		constexpr auto columns = 2;
		ImGui::Columns(columns, nullptr, false);

		const auto& resourceManager = m_detailsWidget->GetResourceManager();
		const auto& resourceDataMap = resourceManager->GetResourceDataListByType(selectType);

		if (!resourceDataMap.empty())
		{
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
		}

		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::EndPopup();
	}

	return resourceData;
}

bool DetailsObject::OpenResourceHelper(StringView lavel) noexcept
{
	const auto buttonLavel = "*##" + lavel;
	m_searchResourceLavel = "SearchResourceHelper" + lavel;

	if (ImGui::Button(buttonLavel.c_str(), ImVec2(20, 20)))
	{
		ImGui::OpenPopup(m_searchResourceLavel.c_str());
		return true;
	}
	return false;
}

IResource* DetailsObject::LoadResource(ResourceData* resourceData) const noexcept
{
	auto resourceHandle = m_detailsWidget->GetResourceManager()->Load(resourceData);
	resourceHandle->WaitForLoadComplete();

	return resourceHandle->GetResource();
}