/**
* @file	   SceneWidget.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "SceneWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Editor/Widget/Widgets/DetailsWidget.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

void SceneWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	m_eventListener.SetFunction([this](std::any data) {
		m_allRootGameObjects.clear();
		if(auto currentScene = m_world->GetCurrentScene())
		{
			currentScene->GetAllRootGameObjects(m_allRootGameObjects);
		}
	});

	m_eventListener.RegisterToEventManager<UpdateSceneTreeEvent>();
}

void SceneWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2( 0, 80), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(220, 470), ImGuiCond_FirstUseEver);

	const auto windowFlags = 0;

	ImGui::Begin("Hierarchy", nullptr, windowFlags);

	if (auto currentScene = m_world->GetCurrentScene())
	{
		if (ImGui::CollapsingHeader(currentScene->GetAssetName().data(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_DefaultOpen) && !EditorHelper::Get().IsChangePlayMode())
		{
			// show gameObjects
			for (const auto& rootGameObject : m_allRootGameObjects)
			{
				AddGameObjectToTree(rootGameObject);
			}

			ShowGameObjectHelper();
			ShowCreateWindow();

			if (ImGui::IsMouseReleased(0))
			{
				// 選択解除
				if (!m_selectGameObject)
				{
					m_gameObject = nullptr;
				}

				// ゲームオブジェクトなら親子関係の解消のための処理
				if (auto dragGameObject = CatchDragGameObject())
				{
					dragGameObject->GetTransform().SetParent(nullptr);
				}

				if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_RootAndChildWindows))
				{
					if (auto dragModel = CatchDragObject<Model>())
					{
						CreateGameObjectFromModel(dragModel);
					}
				}

				m_selectGameObject = false;
			}
		}
		else if(EditorHelper::Get().IsChangePlayMode())
		{
			// モード切り替え時の出力バグ回避のため
			m_allRootGameObjects.clear();

			CancelEvent<UpdateSceneTreeEvent>(false);
			NotifyEvent<UpdateSceneTreeEvent>();
		}
	}

	ImGui::End();
}

void SceneWidget::Serialized(FileStream* file) const
{
	if (!m_world)
	{
		return;
	}

	if (auto scene = m_world->GetCurrentScene())
	{
		file->Write(scene->GetAssetName());
	}
	else
	{
		file->Write(String(""));
	}
}

void SceneWidget::Deserialized(FileStream* file)
{
	String sceneName;
	file->Read(&sceneName);

	if (!sceneName.empty())
	{
		m_world->ChangeScene(sceneName);
	}
}

void SceneWidget::AddGameObjectToTree(GameObject* gameObject) noexcept
{
	auto flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (gameObject->GetTransform().GetChildCount() == 0)
	{
		flags = ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), flags))
	{
		// show select UI
		if (m_gameObject == gameObject)
		{
			ShowSelectUI();
		}

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

void SceneWidget::ShowGameObjectHelper() noexcept
{
	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("GameObject Helper");
	}

	bool isCreate = false;
	bool isDelete = m_gameObject && ImGui::IsKeyPressed(ImGuiKey_Delete);
	if (ImGui::BeginPopup("GameObject Helper"))
	{
		if (ImGui::Button("Create GameObject"))
		{
			isCreate = true;
			ImGui::CloseCurrentPopup();
		}

		if (m_gameObject && ImGui::Button("Delete GameObject"))
		{
			isDelete = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (isCreate)
	{
		ImGui::OpenPopup("Create GameObject");
	}

	if (isDelete)
	{
		m_world->DestroyGameObject(m_gameObject);
		m_gameObject = nullptr;
		DetailsWidget::ClearSelectObject();
	}
}

void SceneWidget::ShowCreateWindow() noexcept
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

void SceneWidget::ShowSelectUI() const noexcept
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	auto min = ImGui::GetItemRectMin();
	auto max = ImGui::GetItemRectMax();

	// 少し大きめの枠にする
	min.x -= 1; max.x = ImGui::GetWindowWidth() - 8;
	min.y -= 1; max.y += 1;

	// 細目の白いライン表示
	draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 180), 0, 0, 1);
}

void SceneWidget::ChackClickedCommand(GameObject* gameObject) noexcept
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
		if (auto dragGameObject = CatchDragGameObject())
		{
			// 違うオブジェクトにドラッグされたら親子関係を構築
			if (dragGameObject == gameObject)
			{
				m_gameObject = gameObject;
				DetailsWidget::SelectGameObject(gameObject);
			}
			else
			{
				dragGameObject->GetTransform().SetParent(&gameObject->GetTransform());
			}
		}

		m_selectGameObject = true;
	}
}

void SceneWidget::CreateGameObjectFromModel(Model* model) noexcept
{
	auto rootGameObject = m_world->CreateGameObject();
	auto& rootTransform = rootGameObject->GetTransform();
	rootGameObject->SetName(model->GetAssetName());

	auto meshes = model->GetAllMeshes();
	if (meshes.size() == 1)
	{
		if (auto component = rootGameObject->AddComponent("IMeshRender"))
		{
			auto meshComponent = dynamic_cast<MeshRender*>(component);
			meshComponent->SetMesh(meshes.front());
		}
	}
	else
	{
		for (auto mesh : model->GetAllMeshes())
		{
			auto gameObject = m_world->CreateGameObject();
			gameObject->SetName(mesh->GetAssetName());
			gameObject->GetTransform().SetParent(&rootTransform);

			if (auto component = gameObject->AddComponent("IMeshRender"))
			{
				auto meshComponent = dynamic_cast<MeshRender*>(component);
				meshComponent->SetMesh(mesh);
			}
		}
	}
}

GameObject* SceneWidget::CatchDragGameObject() const noexcept
{
	if (DragDrop::Get().HasGameObject())
	{
		auto dragObject = DragDrop::Get().GetDragObject();
		DragDrop::Get().EndDrag();

		return std::any_cast<GameObject*>(dragObject);
	}
	return nullptr;
}

ResourceData* SceneWidget::CatchDragResourceData() const noexcept
{
	if (DragDrop::Get().HasResource())
	{
		auto dragObject = DragDrop::Get().GetDragObject();

		return std::any_cast<ResourceData*>(dragObject);
	}
	return nullptr;
}

IResource* SceneWidget::CatchDragResourceObject(uint32_t selectType) const noexcept
{
	if (!DragDrop::Get().HasResource())
	{
		return nullptr;
	}

	const auto resourceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
	const auto type = resourceData->m_resourcePath.m_type;

	if (type != selectType)
	{
		return nullptr;
	}

	// Load Resource
	auto resourceHandle = m_resourceManager->Load(resourceData);
	resourceHandle->WaitForLoadComplete();
	
	return resourceHandle->GetResource();
}