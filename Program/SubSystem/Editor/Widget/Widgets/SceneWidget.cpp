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
	ImGui::SetNextWindowSize(ImVec2(220, 470), ImGuiCond_Once);

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (auto currentScene = m_world->GetCurrentScene())
	{
		for (const auto& gameObject : currentScene->GetAllGameObjects())
		{
			AddGameObjectToTree(gameObject.get());
		}
	}

	ChackClickedCommand();
	ShowGameObjectHelper();
	ShowGameObjectCreateWindow();

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

void SceneWidget::ShowGameObjectHelper() noexcept
{
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
		ImGui::OpenPopup("Create GameObject Window");
	}
}

void SceneWidget::ShowGameObjectCreateWindow()
{
	if (ImGui::BeginPopup("Create GameObject Window"))
	{
		ImGui::Text("Object Name"); ImGui::SameLine();

		char str[128] = "";
		auto isCreate = ImGui::InputTextWithHint("", "none", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行用
		auto isCancel = ImGui::Button("Cancel");

		if (isCreate && !!m_world->GetCurrentScene())
		{
			auto gameObject = m_world->CreateGameObject();
			gameObject->SetName(str);
		}

		if (isCreate || isCancel)
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void SceneWidget::ChackClickedCommand() noexcept
{
	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("GameObject Helper");
	}
}