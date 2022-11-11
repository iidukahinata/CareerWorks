/**
* @file	   ComponentDetails.cpp
* @brief
*
* @date	   2022/10/27 2022”N“x‰”Å
*/


#include "ComponentDetails.h"
#include "SubSystem/Scene/GameObject.h"
#include "SubSystem/Scene/Component/IComponent.h"
#include "ThirdParty/imgui/imgui_internal.h"

ComponentDetails::ComponentDetails(DetailsWidget* detailsWidget) :
	DetailsObject(detailsWidget) 
{

}

ComponentDetails::~ComponentDetails()
{

}

bool ComponentDetails::ShowComponentHeader(IComponent* component) noexcept
{
	auto name		   = component->GetTypeData().Name;
	auto gameObject	   = component->GetOwner();
	auto isActive	   = component->GetActive();
	auto buttonActive  = String("##Active") + name.data();
	auto buttonLavel   = String("=##") + name.data();
	auto settginsLavel = String("Component Settings##") + name.data();

	auto GImGui = ImGui::GetCurrentContext();
	auto contentRegionAvailable = ImGui::GetContentRegionAvail();
	auto lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

	const ImGuiTreeNodeFlags treeNodeFlags =
		ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
		ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

	bool isOpen = ImGui::CollapsingHeader(name.data(), treeNodeFlags);

	ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
	if (ImGui::Button(buttonLavel.c_str(), ImVec2(lineHeight, lineHeight)))
	{
		ImGui::OpenPopup(settginsLavel.c_str());
	}

	ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.7f);
	auto inputActive = ImGui::Checkbox(buttonActive.c_str(), &isActive);

	auto inputRemove = false;
	if (ImGui::BeginPopup(settginsLavel.c_str()))
	{
		inputRemove = ImGui::Button("Remove Component");
		ImGui::EndPopup();
	}

	if (inputActive) { RegisterEditorCommand([component](auto data) { component->SetActive(data);  }, isActive, component->GetActive()); }
	if (inputRemove) { gameObject->RemoveComponent(component); m_detailsWidget->RequestUpdate(); }

	return isOpen && !inputRemove;
}