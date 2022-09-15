/**
* @file	   GameObjectDetails.cpp
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/


#include "GameObjectDetails.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"

GameObjectDetails::GameObjectDetails(DetailsWidget* detailsWidget, GameObject* gameObject) :
	DetailsObject(detailsWidget), m_gameObject(gameObject)
{

}

void GameObjectDetails::Draw()
{
	constexpr int offsetPos = 130;

	ImGui::SameLine(70);
	ImGui::Button("Add Component", ImVec2(300, 50));

	ImGui::Separator();
	ImGui::Text("Name"); ImGui::SameLine(offsetPos);
	ImGui::Text(m_gameObject->GetName().c_str());
}