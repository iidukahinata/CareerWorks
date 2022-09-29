/**
* @file	   AssetsWidget.cpp
* @brief
*
* @date	   2022/09/27 2022年度初版
*/


#include "AssetsWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Input/Input.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Shader.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/3DModel/Texture.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"

void AssetsWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	m_currentDirectory = "Data/asset";
	m_directoryTree.emplace_back("asset");
	NavigateToDirectory(m_currentDirectory);
}

void AssetsWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2( 0, 530), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(755, 333), ImGuiCond_Once);

	ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	ShowResourceMenu();
	ShowCurrentDirectoryResourceList();
	//ShowResourceList();

	ChackClickedCommand();

	// Draw Helper Window
	ShowResourceHelper();

	ImGui::End();
}

void AssetsWidget::ShowResourceMenu() noexcept
{
	// tag 検索用 Filter 定義
	if (ImGui::TreeNode("Filter"))
	{
		auto isChangeTag = false;

		ImGui::Columns(3, nullptr, false);

		isChangeTag |= ImGui::RadioButton("ALL", &m_tag, TagType_ALL);
		ImGui::NextColumn();

#define CREATE_RADIO_BUTTON_FROM_TYPE(CLASS)												  \
isChangeTag |= ImGui::RadioButton(CLASS::TypeData.Name.data(), &m_tag, CLASS::TypeData.Hash); \
ImGui::NextColumn();

		// タグ指定用ボタンの作成
		CREATE_RADIO_BUTTON_FROM_TYPE(Scene);
		CREATE_RADIO_BUTTON_FROM_TYPE(Model);
		CREATE_RADIO_BUTTON_FROM_TYPE(Mesh);
		CREATE_RADIO_BUTTON_FROM_TYPE(Material);
		CREATE_RADIO_BUTTON_FROM_TYPE(Shader);
		CREATE_RADIO_BUTTON_FROM_TYPE(Texture);
		CREATE_RADIO_BUTTON_FROM_TYPE(AudioClip);

		ImGui::TreePop();
		ImGui::Columns(1);

		if (isChangeTag)
		{
			NavigateToDirectory(m_currentDirectory);
		}
	}
	else
	{
		ImGui::SameLine();
	}

	// 文字列検索用 Filter 定義
	m_filter.Draw("Search", 200.0f);

	auto numTree = m_directoryTree.size();
	for (int i = 0; i < numTree; ++i)
	{
		if (i != 0)
		{
			if (ImGui::Button(">"))
			{
				for (int index = 0; index < numTree - i; ++index)
				{
					CurrentDirectoryToParent();
				}
				break;
			}

			ImGui::SameLine();
		}

		ImGui::Text(m_directoryTree[i].c_str()); ImGui::SameLine();
	}

	ImGui::Text("");
}

void AssetsWidget::ShowCurrentDirectoryResourceList() noexcept
{
	const auto childWidth = ImGui::GetWindowContentRegionWidth();
	const auto childHeight = ImGui::GetWindowHeight() - 60;
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(childWidth, childHeight), true, ImGuiWindowFlags_NoScrollbar);

	const auto columns = static_cast<int>(std::round(max(ImGui::GetWindowContentRegionWidth() / 110.f, 1.f)));
	ImGui::Columns(columns, nullptr, false);

	for (const auto& entries : m_directoryEntries)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.5, 0.5, 0.8));

		auto iconTexture = EditorHelper::Get().GetIconTexture(entries.type);
		EditorHelper::Get().AddImageButton(iconTexture->GetData(), ImVec2(90, 90));

		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			// ドラッグアンドドロップのために Down 時判定
			if (ImGui::IsMouseDown(0) && m_selectResourceName != entries.name)
			{
				m_stopwatch.Start();
				ClickResource(entries.type, entries.name);
			}

			else if (ImGui::IsMouseDoubleClicked(0))
			{
				DoubleClickResource(entries.type, entries.name);
			}

			// 入力終了時の選択を Details に表示させるようにするため
			else if (ImGui::IsMouseReleased(0))
			{
				DetailsWidget::SelectResource(m_selectResoruce);
			}
		}

		auto&& label = ConvertToJapanese(entries.name);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
	}

	if (m_isSelectDirectory)
	{
		NavigateToDirectory(m_currentDirectory);
		m_isSelectDirectory = false;
	}

	if (IsTimeOut())
	{
		m_stopwatch.Start();
		m_selectResourceName = String();
		m_selectResoruce = nullptr;
	}

	ImGui::Columns(1);
	ImGui::EndChild();
}

void AssetsWidget::ShowResourceList() noexcept
{
	const auto childWidth = ImGui::GetWindowContentRegionWidth();
	const auto childHeight = ImGui::GetWindowHeight() - 60;
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(childWidth, childHeight), true, ImGuiWindowFlags_NoScrollbar);

	const auto columns = static_cast<int>(std::round(max(ImGui::GetWindowContentRegionWidth() / 130.f, 1.f)));
	ImGui::Columns(columns, nullptr, false);

	for (const auto& entries : m_directoryEntries)
	{
		// 指定文字列を含む Resource のみ列挙
		if (!m_filter.PassFilter(entries.name.c_str()))
		{
			continue;
		}

		auto iconTexture = EditorHelper::Get().GetIconTexture(entries.type);

		EditorHelper::Get().AddImageButton(iconTexture->GetData(), ImVec2(120, 120));

		if (ImGui::IsItemHovered())
		{
			// ドラッグアンドドロップのために Down 時判定
			if (ImGui::IsMouseDown(0) && m_selectResourceName != entries.name)
			{
				m_stopwatch.Start();
				ClickResource(entries.type, entries.name);
			}
			else if (ImGui::IsMouseDoubleClicked(0))
			{
				DoubleClickResource(entries.type, entries.name);
			}

			// 入力終了時の選択を Details に表示させるようにするため
			if (ImGui::IsMouseClicked(0))
			{
				DetailsWidget::SelectResource(m_selectResoruce);
			}
		}

		auto&& label = ConvertToJapanese(entries.name);

		auto m_itemSize = 120;

		ImGui::Text(label.c_str());

		ImGui::NextColumn();
	}

	if (m_isSelectDirectory)
	{
		NavigateToDirectory(m_currentDirectory);
		m_isSelectDirectory = false;
	}

	// time out
	if (IsTimeOut())
	{
		m_stopwatch.Start();
		m_selectResourceName = String();
		m_selectResoruce = nullptr;
	}

	ImGui::Columns(1);
	ImGui::EndChild();
}

void AssetsWidget::ShowResourceHelper() noexcept
{
	ImGui::SetNextWindowPos(m_clickedMausePos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(150, 200), ImGuiCond_Always);

	if(ImGui::BeginPopup("Assets Helper", 0))
	{
		if (ImGui::Button("Create Folder", ImVec2(135, 20)))
		{
			ImGui::OpenPopup("Create Folder");
		}

		if (ImGui::Button("Create Scene", ImVec2(135, 20)))
		{
			ImGui::OpenPopup("Create Resource");
			m_resourceCreateFunc = [](StringView name) { return Scene::Create(name); };
		}

		if (ImGui::Button("Create Material", ImVec2(135, 20)))
		{
			ImGui::OpenPopup("Create Resource");
			m_resourceCreateFunc = [](StringView name) { return Material::Create(name); };
		}

		bool isClosePopup = false;

		// Draw Create Folder
		auto center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(ImVec2(center.x - 150, center.y), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);

		if (ImGui::BeginPopupModal("Create Folder"))
		{
			isClosePopup = ShowCreateWindow([this](StringView name) {

			});
		}

		// Draw Create Resource
		ImGui::SetNextWindowPos(ImVec2(center.x - 150, center.y), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);

		if (ImGui::BeginPopupModal("Create Resource", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			isClosePopup = ShowCreateWindow([this](StringView name) {

				auto resource = m_resourceCreateFunc(m_currentDirectory + name.data());

				// リソースの生成だけが目的のため解放
				UnloadResource(resource);
			});
		}

		if (isClosePopup)
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

bool AssetsWidget::ShowCreateWindow(std::function<void(StringView)> createFunc) noexcept
{
	ImGui::Text("Object Name"); ImGui::SameLine();

	char name[128] = "";
	auto isCreate = ImGui::InputTextWithHint("", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行
	auto isCancel = ImGui::Button("Cancel");

	if (isCreate)
	{
		createFunc(name);
	}

	if (isCreate || isCancel)
	{
		m_resourceCreateFunc = nullptr;
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();

	return isCreate || isCancel;
}

void AssetsWidget::ChackClickedCommand() noexcept
{
	if (ImGui::IsMouseClicked(1))
	{
		ImGui::OpenPopup("Assets Helper");
		m_clickedMausePos	 = ImGui::GetMousePos();
	}

	// delete resource
	if (m_selectResoruce && ImGui::IsKeyReleased(VK_DELETE))
	{
		
	}
}

void AssetsWidget::UnloadResource(IResource* resource) noexcept
{
	if (auto resourceData = m_resourceManager->GetResourceData(resource->GetFilePath()))
	{
		m_resourceManager->Unload(resourceData);
	}
}

void AssetsWidget::ClickResource(IconType type, StringView name) noexcept
{
	if (type == Icon_Folder)
	{
		m_selectResourceName = name;
	}
	else
	{
		const auto resourceType = EditorHelper::Get().GetResourceTypeByIconType(type);

		if (auto resourceData = m_resourceManager->GetResourceData(resourceType, name))
		{
			m_selectResourceName = name;
			m_selectResoruce = resourceData;
			DragDrop::Get().StartDrag(DragDrop_Resource, resourceData);
		}
	}
}

void AssetsWidget::DoubleClickResource(IconType type, StringView name) noexcept
{
	if (type == Icon_Folder)
	{
		m_directoryTree.emplace_back(name);
		m_currentDirectory += String("/") + name.data();
		m_isSelectDirectory = true;
	}
	else
	{
		const auto resourceType = EditorHelper::Get().GetResourceTypeByIconType(type);

		if (resourceType == Scene::TypeData.Hash)
		{
			// シーン切り替えでリソースが消去されるため
			DetailsWidget::ClearSelectObject();
			EditorHelper::Get().FlushCommandList();

			m_world->ChangeScene(name);
		}
	}
}

void AssetsWidget::NavigateToDirectory(StringView path)
{
	m_directoryEntries.clear();
	m_directoryEntries.shrink_to_fit();

	for (const auto& directory : FileSystem::GetDirectorysFromDirectory(path))
	{
		m_directoryEntries.emplace_back(Thumbnail(FileSystem::GetFilePath(directory), Icon_Folder));
	}

	Vector<std::pair<String, ResourceData*>> resourceDatas;
	m_resourceManager->GetResourceDataListFromDirectory(path, resourceDatas);

	for (const auto& resourceInfo : resourceDatas)
	{
		const auto name = resourceInfo.first;
		const auto type = resourceInfo.second->m_resourcePath.m_type;

		if (m_tag != TagType_ALL && m_tag != type)
		{
			continue;
		}

		const auto iconType = EditorHelper::Get().GetIconTypeFromResourceType(type);

		m_directoryEntries.emplace_back(Thumbnail(name, iconType));
	}
}

void AssetsWidget::CurrentDirectoryToParent() noexcept
{
	m_directoryTree.pop_back();
	m_currentDirectory = FileSystem::GetParentDirectory(m_currentDirectory);
	m_isSelectDirectory = true;
}

bool AssetsWidget::IsTimeOut() noexcept
{
	constexpr int timeout = 300;
	return m_stopwatch.GetRap(Milli) >= timeout;
}