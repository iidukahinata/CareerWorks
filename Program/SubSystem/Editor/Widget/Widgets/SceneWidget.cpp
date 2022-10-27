/**
* @file	   SceneWidget.cpp
* @brief
*
* @date	   2022/10/21 2022年度初版
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
	ImGui::SetNextWindowPos(ImVec2( 0, 80), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(220, 470), ImGuiCond_FirstUseEver);

	const auto windowFlags = 0;

	ImGui::Begin("Hierarchy", nullptr, windowFlags);

	if (auto currentScene = m_world->GetCurrentScene())
	{
		if (ImGui::CollapsingHeader(currentScene->GetAssetName().data(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_DefaultOpen))
		{
			// show gameObjects
			Vector<GameObject*> allRootGameObjects;
			currentScene->GetAllRootGameObjects(allRootGameObjects);
			for (const auto& rootGameObject : allRootGameObjects)
			{
				AddGameObjectToTree(rootGameObject);
			}

			ShowGameObjectHelper();
			ShowGameObjectCreateWindow();
		}
	}

	ImGui::End();
}

void SceneWidget::AddGameObjectToTree(GameObject* gameObject) const noexcept
{
	auto flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (gameObject->GetTransform().GetChildCount() == 0)
	{
		flags = ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), flags))
	{
		ChackClickedCommand(gameObject);

		// show children
		for (auto& child : gameObject->GetTransform().GetChildren())
		{
			AddGameObjectToTree(child->GetOwner());
		}

		ImGui::TreePop();
	}
	else
	{
		ChackClickedCommand(gameObject);
	}
}

void SceneWidget::ShowGameObjectHelper() const noexcept
{
	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("GameObject Helper");
	}

	bool isCreate = false;
	if (ImGui::BeginPopup("GameObject Helper"))
	{
		if (ImGui::Button("Create GameObject"))
		{
			isCreate = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (isCreate)
	{
		ImGui::OpenPopup("Create GameObject");
	}
}

void SceneWidget::ShowGameObjectCreateWindow() noexcept
{
	ASSERT(m_world->GetCurrentScene());

	const auto center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(ImVec2(center.x - 150, center.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);

	if (ImGui::BeginPopupModal("Create GameObject"))
	{
		ImGui::Text("Object Name"); ImGui::SameLine();

		char name[128] = "";
		auto isCreate = ImGui::InputTextWithHint("##GameObjectName", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行用
		auto isCancel = ImGui::Button("Cancel");

		if (isCreate)
		{
			auto gameObject = m_world->CreateGameObject();
			gameObject->SetName(name);
		}

		if (isCreate || isCancel)
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void SceneWidget::ChackClickedCommand(GameObject* gameObject) const noexcept
{
	// ドラッグアンドドロップ有効指定
	if (!ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
	{
		return;
	}

	// 矢印が押された時は反応しない
	if (ImGui::IsItemToggledOpen())
	{
		return;
	}

	if (ImGui::IsMouseClicked(0))
	{
		DragDrop::Get().StartDrag(DragDrop_GameObject, gameObject);
	}

	if (ImGui::IsMouseReleased(0))
	{
		if (DragDrop::Get().HasGameObject())
		{			
			auto dragGameObject = CatchDragObject();
			if (dragGameObject == gameObject)
			{
				DetailsWidget::SelectGameObject(gameObject);
			}
			else
			{
				dragGameObject->GetTransform().SetParent(&gameObject->GetTransform());
			}
		}
	}
}

GameObject* SceneWidget::CatchDragObject() const noexcept
{
	if (DragDrop::Get().HasGameObject())
	{
		auto dragObject = DragDrop::Get().GetDragObject();
		return std::any_cast<GameObject*>(dragObject);
	}
	return nullptr;
}