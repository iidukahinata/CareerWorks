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

	// 細目の白いライン表示
	draw_list->AddRect(min, max, IM_COL32(150, 150, 150, 120), 0, 0, 1);

	if (!DragDrop::Get().HasResource())
	{
		return;
	}

	const auto resrouceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
	const auto type = resrouceData->m_resourcePath.m_type;

	// 指定型のみ受付する
	if (type == selctType)
	{
		auto alpha = hovered ? 255 : 153;
		draw_list->AddRect(min, max, IM_COL32(r, g, b, alpha), 0, 0, 3);
	}
	// 赤い禁止するような UI 表示
	else
	{
		min.y += 10; max.y -= 10;
		draw_list->AddRect(min, max, IM_COL32(200, 0, 0, 180), 0, ImDrawFlags_RoundCornersAll, 3);
	}
}

IResource* DetailsObject::CatchDragObject(uint32_t selectType) const noexcept
{
	if (!DragDrop::Get().HasResource())
	{
		return nullptr;
	}

	const auto resrouceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
	const auto type = resrouceData->m_resourcePath.m_type;

	if (type != selectType)
	{
		return nullptr;
	}

	auto resourceHandle = m_detailsWidget->GetResourceManager()->Load(resrouceData);

	resourceHandle->WaitForLoadComplete();
	return resourceHandle->GetResource();
}