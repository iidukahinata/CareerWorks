/**
* @file	   ResourceDetails.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
*/


#include "ResourceDetails.h"
#include "SubSystem/Resource/ResourceData.h"

ResourceDetails::ResourceDetails(DetailsWidget* detailsWidget, ResourceData* resourceData) :
	DetailsObject(detailsWidget), m_resourceData(resourceData)
{

}

void ResourceDetails::Draw()
{
	constexpr int offsetPos = 130;

	if (ImGui::CollapsingHeader("Resource Data", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto&& path = ConvertToJapanese(m_resourceData->m_resourcePath.m_path);

		ImGui::Text("Name"); ImGui::SameLine(offsetPos);
		ImGui::Text(path.c_str()); ImGui::Text("");

		ImGui::Text("Ref Resource Names");

		// 参照リソースの表示
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(405, 250), ImGuiWindowFlags_NoTitleBar);
		for (auto& resourcePath : m_resourceData->m_refResourcePaths)
		{
			auto&& refPath = ConvertToJapanese(resourcePath.m_path);

			ImGui::Text(refPath.c_str());
		}
		ImGui::EndChild();
	}
}