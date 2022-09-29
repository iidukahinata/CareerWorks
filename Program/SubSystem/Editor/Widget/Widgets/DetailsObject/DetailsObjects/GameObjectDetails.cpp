/**
* @file	   GameObjectDetails.cpp
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
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
	RegisterComponent<Light>();
	RegisterComponent<Camera>();
	RegisterComponent<ModelRender>();
	RegisterComponent<AudioSpeaker>();
	RegisterComponent<AudioListener>();
}

void GameObjectDetails::Draw()
{
	constexpr int offsetPos = 130;

	ImGui::SameLine(70);
	if (ImGui::Button("Add Component", ImVec2(300, 50)))
	{
		ImGui::OpenPopup("AddComponent");
	}

	ShowAddComponentWindow();

	ImGui::Separator();
	ImGui::Text("Name"); ImGui::SameLine(offsetPos);
	ImGui::Text(m_gameObject->GetName().c_str());
}

void GameObjectDetails::ShowAddComponentWindow() noexcept
{
	if (ImGui::BeginPopup("AddComponent"))
	{
		ImGui::Text("Component Name");
		m_filter.Draw("Filter", 150.0f);

		ImGui::BeginChild("##Components", ImVec2(180, 500));

		for (auto& component : m_components)
		{
			auto name = component->GetTypeData().Name;

			if (!m_filter.PassFilter(name.data()))
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