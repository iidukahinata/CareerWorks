/**
* @file	   GameObjectDetails.cpp
* @brief
*
* @date	   2022/10/27 2022年度初版
*/


#include "GameObjectDetails.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"

#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/Light.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Scene/Component/Components/AudioSpeaker.h"
#include "SubSystem/Scene/Component/Components/AudioListener.h"

GameObjectDetails::GameObjectDetails(DetailsWidget* detailsWidget, GameObject* gameObject) :
	DetailsObject(detailsWidget), m_gameObject(gameObject)
{
	// Add Component 時の検索に使用
	RegisterComponent<Light>();
	RegisterComponent<Camera>();
	RegisterComponent<ModelRender>();
	RegisterComponent<AudioSpeaker>();
	RegisterComponent<AudioListener>();
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

		for (const auto& component : m_components)
		{
			auto name = component->GetTypeData().Name;

			if (!m_filter.PassFilter(name.data()))
				continue;

			if (m_gameObject->FindComponent(name))
				continue;

			if (ImGui::Button(name.data(), ImVec2(180, 20)))
			{
				m_gameObject->AddComponent(name);
				m_detailsWidget->RequestUpdate();
			}
		}

		ImGui::EndChild();
		ImGui::EndPopup();
	}
}