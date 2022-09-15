/**
* @file	   SceneWidget.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
*/


#include "SceneWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Editor/Widget/Widgets/DetailsWidget.h"

void SceneWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);
}

void SceneWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2( 0, 80), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 450), ImGuiCond_Once);

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	if (auto currentScene = m_world->GetCurrentScene())
	{
		for (const auto& gameObject : currentScene->GetAllGameObjects())
		{
			AddGameObjectToTree(gameObject.get());
		}
	}

	ImGui::End();
}

void SceneWidget::AddGameObjectToTree(GameObject* gameObject) noexcept
{
	auto flags = 0;
	if (gameObject->GetTransform().GetChildCount() == 0)
	{
		flags = ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), flags))
	{
		// ドラッグアンドドロップ有効指定
		const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

		// ドラッグアンドドロップでの Texture 切り替えのため
		if (ImGui::IsMouseReleased(0) && hoverd)
		{
			SelectGameObject(gameObject);
		}

		ImGui::TreePop();
	}
}

void SceneWidget::SelectGameObject(GameObject* gameObject) noexcept
{
	if (DragDrop::Get().HasDragObject())
	{

	}
	else
	{
		DetailsWidget::SelectGameObject(gameObject);
	}
}