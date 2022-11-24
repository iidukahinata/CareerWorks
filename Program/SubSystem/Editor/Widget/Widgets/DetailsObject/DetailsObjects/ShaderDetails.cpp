/**
* @file	   ShaderDetails.cpp
* @brief
*
* @date	   2022/11/24 2022”N“x‰”Å
*/


#include "ShaderDetails.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

ShaderDetails::ShaderDetails(DetailsWidget* detailsWidget, ResourceData* resourceData) :
	DetailsObject(detailsWidget), m_resourceData(resourceData)
{

}

void ShaderDetails::Draw()
{
	constexpr int offsetPos = 130;
	constexpr int childoffset = 15;

	if (ImGui::CollapsingHeader("Shader Data", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto&& path = ConvertToJapanese(m_resourceData->m_resourcePath.m_path);
		auto width = ImGui::GetWindowWidth() - childoffset;

		ImGui::Text("Name"); ImGui::SameLine(offsetPos);
		ImGui::Text(path.c_str()); ImGui::Text("");

		if (ImGui::Button("Refresh Shader (Build)"))
		{
			StringView thisPath = m_resourceData->m_resourcePath.m_path;

			auto&& resourceList = m_detailsWidget->GetResourceManager()->GetResourcesByType<Material>();
			for (const auto& resource : resourceList)
			{
				auto material = dynamic_cast<Material*>(resource);
				auto vertexPath = material->GetShaderPaths()[VertexShader];

				if (thisPath == vertexPath.c_str())
				{
					material->RefreshShader();
				}
			}
		}
	}
}