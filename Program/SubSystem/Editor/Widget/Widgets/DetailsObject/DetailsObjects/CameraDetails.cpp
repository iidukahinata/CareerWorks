/**
* @file	   CameraDetails.cpp
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/

#include "CameraDetails.h"
#include "SubSystem/Scene/Component/Components/Camera.h"

CameraDetails::CameraDetails(DetailsWidget* detailsWidget, IComponent* component) : 
	DetailsObject(detailsWidget)
{
	m_camera = dynamic_cast<Camera*>(component);
	ASSERT(m_camera);
}

void CameraDetails::Draw()
{
	constexpr int offsetPos = 130;
	const auto& name = m_camera->TypeData.Name;

	if (ImGui::CollapsingHeader(name.data(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto widht	= m_camera->GetWidth();
		auto height = m_camera->GetHeight();
		auto aspect = m_camera->GetAspect();
		auto fov	= m_camera->GetFov();
		auto nearZ	= m_camera->GetNear();
		auto farZ	= m_camera->GetFar();

		ImGui::Text("Widht"); ImGui::SameLine(offsetPos);
		auto inputWidht = ImGui::DragFloat("##Widht", &widht);

		ImGui::Text("Height"); ImGui::SameLine(offsetPos);
		auto inputHeight = ImGui::DragFloat("##Height", &height);

		ImGui::Text("Aspect"); ImGui::SameLine(offsetPos);
		auto inputAspect = ImGui::DragFloat("##Aspect", &aspect);

		ImGui::Text("Fov"); ImGui::SameLine(offsetPos);
		auto inputFov = ImGui::DragFloat("##Fov", &fov);

		ImGui::Text("Near"); ImGui::SameLine(offsetPos);
		auto inputNear = ImGui::DragFloat("##Near", &nearZ);

		ImGui::Text("Far"); ImGui::SameLine(offsetPos);
		auto inputFar = ImGui::DragFloat("##Far", &farZ);

		if (inputWidht)	 m_camera->SetWidth(widht);
		if (inputHeight) m_camera->SetHeight(height);
		if (inputAspect) m_camera->SetAspect(aspect);
		if (inputFov)	 m_camera->SetFov(fov);
		if (inputNear)	 m_camera->SetNear(nearZ);
		if (inputFar)	 m_camera->SetFar(farZ);
	}
}