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
		auto camera = m_camera;
		auto widht	= camera->GetWidth();
		auto height = camera->GetHeight();
		auto aspect = camera->GetAspect();
		auto fov	= camera->GetFov();
		auto nearZ	= camera->GetNear();
		auto farZ	= camera->GetFar();

		ImGui::Text("Widht"); ImGui::SameLine(offsetPos);
		auto inputWidht = ImGui::DragFloat("##Widht", &widht);

		ImGui::Text("Height"); ImGui::SameLine(offsetPos);
		auto inputHeight = ImGui::DragFloat("##Height", &height);

		ImGui::Text("Aspect"); ImGui::SameLine(offsetPos);
		auto inputAspect = ImGui::DragFloat("##Aspect", &aspect);

		ImGui::Text("Fov"); ImGui::SameLine(offsetPos);
		auto inputFov = ImGui::DragFloat("##Fov", &fov);

		ImGui::Text("Near"); ImGui::SameLine(offsetPos);
		auto inputNear = ImGui::DragFloat("##Near", &nearZ, 0.1f);

		ImGui::Text("Far"); ImGui::SameLine(offsetPos);
		auto inputFar = ImGui::DragFloat("##Far", &farZ, 0.1f);

		if (inputWidht)	 RegisterEditorCommand([camera](auto data) { camera->SetWidth(data);  }, widht , camera->GetWidth());
		if (inputHeight) RegisterEditorCommand([camera](auto data) { camera->SetHeight(data); }, height, camera->GetHeight());
		if (inputAspect) RegisterEditorCommand([camera](auto data) { camera->SetAspect(data); }, aspect, camera->GetAspect());
		if (inputFov)	 RegisterEditorCommand([camera](auto data) { camera->SetFov(data);	  }, fov   , camera->GetFov());
		if (inputNear)	 RegisterEditorCommand([camera](auto data) { camera->SetNear(data);   }, nearZ , camera->GetNear());
		if (inputFar)	 RegisterEditorCommand([camera](auto data) { camera->SetFar(data);	  }, farZ  , camera->GetFar());
	}
}