/**
* @file	   GameObjectDetails.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "GameObjectDetails.h"
#include "SubSystem/Scene/GameObject.h"
#include "SubSystem/Scene/Factory/ComponentFactory.h"

GameObjectDetails::GameObjectDetails(DetailsWidget* detailsWidget, GameObject* gameObject) :
	DetailsObject(detailsWidget), m_gameObject(gameObject)
{

}

void GameObjectDetails::Draw()
{
	constexpr int offsetPos = 130;
	auto width = ImGui::GetWindowWidth() - offsetPos;

	ImGui::SameLine(65);
	if (ImGui::Button("Add Component", ImVec2(width, 50)))
	{
		ImGui::OpenPopup("AddComponent");
	}

	ShowAddComponentWindow();

	ImGui::Separator();

	auto gameObject = m_gameObject;
	auto name		= m_gameObject->GetName();
	auto active		= m_gameObject->GetActive();
	name.resize(128);

	ImGui::Text("Name"); ImGui::SameLine(offsetPos);
	auto inputName = ImGui::InputText("##Object Name", name.data(), name.size());

	ImGui::Text("Active"); ImGui::SameLine(offsetPos);
	auto inputActive = ImGui::Checkbox("##Active Mode", &active);

	if (inputName)   RegisterEditorCommand([gameObject](auto data) { gameObject->SetName(data); }, name, gameObject->GetName());
	if (inputActive) RegisterEditorCommand([gameObject](auto data) { gameObject->SetActive(data); }, active, gameObject->GetActive());
}

void GameObjectDetails::ShowAddComponentWindow() noexcept
{
	if (ImGui::BeginPopup("AddComponent"))
	{
		ImGui::Text("Component Name");
		m_filter.Draw("Filter", 150.0f);

		ImGui::BeginChild("##Components", ImVec2(180, 500));

		auto typeList = ComponentFactory::GetAllComponentType();
		for (auto type : typeList)
		{
			if (!m_filter.PassFilter(type.data()))
				continue;

			if (m_gameObject->FindComponent(type))
				continue;

			if (ImGui::Button(type.data(), ImVec2(180, 20)))
			{
				m_gameObject->AddComponent(type);
				m_detailsWidget->RequestUpdate();
			}
		}

		ImGui::EndChild();
		ImGui::EndPopup();
	}
}