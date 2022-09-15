/**
* @file	   ResourceWidget.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
*/


#include "ResourceWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Input/Input.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/3DModel/Texture.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"

void ResourceWidget::Initialize()
{
	RegisterResourceColor();
}

void ResourceWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);
}

void ResourceWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2( 0, 530), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(740, 333), ImGuiCond_Once);

	ImGui::Begin("Resource List", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	ShowResourceMenu();
	ShowResourceList();

	ImGui::End();

	ChackClickedCommand();

	// Draw Helper Window
	ShowResourceHelper();
	ShowResourceCreateWindow();
}

void ResourceWidget::ShowResourceMenu() noexcept
{
	// tag 検索用 Filter 定義
	if (ImGui::TreeNode("Filter"))
	{
		ImGui::Columns(3, nullptr, false);

		ImGui::RadioButton("ALL", &m_tag, TagType_ALL);
		ImGui::NextColumn();

#define CREATE_RADIO_BUTTON_FROM_TYPE(CLASS)								   \
ImGui::RadioButton(CLASS::TypeData.Name.data(), &m_tag, CLASS::TypeData.Hash); \
ImGui::NextColumn();

		// タグ指定用ボタンの作成
		CREATE_RADIO_BUTTON_FROM_TYPE(Scene);
		CREATE_RADIO_BUTTON_FROM_TYPE(Model);
		CREATE_RADIO_BUTTON_FROM_TYPE(Mesh);
		CREATE_RADIO_BUTTON_FROM_TYPE(Material);
		CREATE_RADIO_BUTTON_FROM_TYPE(Texture);
		CREATE_RADIO_BUTTON_FROM_TYPE(AudioClip);

		ImGui::TreePop();
		ImGui::Columns(1);
	}
	else
	{
		ImGui::SameLine();
	}

	// 文字列検索用 Filter 定義
	m_filter.Draw("Search", 200.0f);
}

void ResourceWidget::ShowResourceList() noexcept
{
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(710, 270), true, ImGuiWindowFlags_NoScrollbar);

	const auto columns = static_cast<int>(ImGui::GetWindowContentRegionWidth() / 120.f);
	ImGui::Columns(columns, nullptr, false);

	// 表示される Type だけ列挙
	Vector<uint32_t> showTypeList;
	for (auto type : m_resourceManager->GetResourceTypes())
	{
		if (IsShowAllResourceType() || m_tag == type)
		{
			showTypeList.emplace_back(type);
		}
	}

	// 全リソース表示ループ
	for (auto type : showTypeList)
	{
		// リソースタイプによって色変更
		ImGui::PushStyleColor(ImGuiCol_Button, m_color[type]);

		auto&& resourceDataMap = m_resourceManager->GetResourceDataListByType(type);
		for (auto resourceInfo : resourceDataMap)
		{
			const auto& assetName = resourceInfo.first;
			const auto& assetData = resourceInfo.second;

			// 指定文字列を含む Resource のみ列挙
			if (!m_filter.PassFilter(assetName.c_str()))
			{
				continue;
			}

			auto&& label = ConvertToJapanese(assetName);

			// ドラッグアンドドロップに対応させるために Push 時に入力感知
			if (ImGui::Button(label.c_str(), ImVec2(130, 120), ImGuiButtonFlags_MusePushTure))
			{
				if (!m_selectResoruce || (*m_selectResoruce) != assetData)
				{
					m_stopwatch.Start();
					ClickResource(type, assetName);
				}
				else if (!IsTimeOut())
				{
					DoubleClickResource(type, assetName);
				}
			}

			// 入力終了時の選択を Details に表示させるようにするため
			if (ImGui::IsMouseReleased(0) && ImGui::IsItemHovered())
			{
				DetailsWidget::SelectResource(m_selectResoruce);
			}

			ImGui::NextColumn();
		}
		ImGui::PopStyleColor();
	}

	// time out
	if (ImGui::IsMouseReleased(0) && IsTimeOut())
	{
		m_selectResoruce = nullptr;
	}

	ImGui::Columns(1);
	ImGui::EndChild();
}

void ResourceWidget::ShowResourceHelper() noexcept
{
	if (!m_isShowHelperWindow)
	{
		return;
	}

	ImGui::SetNextWindowPos(m_clickedMausePos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(150, 200), ImGuiCond_Always);

	ImGui::Begin("Resource Helper", 0, ImGuiWindowFlags_NoTitleBar);

	// Window 以外の場所をクリックした場合消去
	if (!ImGui::IsWindowFocused())
	{
		m_isShowHelperWindow = false;
	}

	if (ImGui::Button("Create Scene", ImVec2(135, 20)))
	{
		m_resourceCreateFunc = [](StringView name) { return Scene::Create(name); };
	}

	if (ImGui::Button("Create Material", ImVec2(135, 20)))
	{
		m_resourceCreateFunc = [](StringView name) { return Material::Create(name); };
	}

	ImGui::End();
}

void ResourceWidget::ShowResourceCreateWindow()
{
	if (!IsShowCreateWindow())
	{
		return;
	}

	ImGui::SetNextWindowPos(m_clickedMausePos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);

	ImGui::Begin("Create Resource", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Resource Name"); ImGui::SameLine();

	char str[128] = "";
	ImGui::InputTextWithHint("", "none", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行用

	const auto isCreate = ImGui::Button("Create"); ImGui::SameLine();
	const auto isCancel = ImGui::Button("Cancel");

	if (isCreate)
	{
		auto resource = m_resourceCreateFunc(str);

		// リソースの生成だけが目的のため解放
		UnloadResource(resource);
		m_resourceCreateFunc = nullptr;
	}

	if (isCancel)
	{
		m_resourceCreateFunc = nullptr;
	}

	ImGui::End();
}

void ResourceWidget::ChackClickedCommand() noexcept
{
	if (ImGui::IsMouseClicked(1))
	{
		m_isShowHelperWindow = true;
		m_clickedMausePos = ImGui::GetMousePos();
	}

	// delete resource
	if (m_selectResoruce && ImGui::IsKeyReleased(VK_DELETE))
	{
		
	}
}

void ResourceWidget::UnloadResource(IResource* resource) noexcept
{
	if (auto resourceData = m_resourceManager->GetResourceData(resource->GetFilePath()))
	{
		m_resourceManager->Unload(resourceData);
	}
}

void ResourceWidget::ClickResource(uint32_t type, StringView name) noexcept
{
	if (auto resourceData = m_resourceManager->GetResourceData(type, name))
	{
		m_selectResoruce = resourceData;
		DragDrop::Get().StartDrag(DragDrop_Resource, resourceData);
	}
}

void ResourceWidget::DoubleClickResource(uint32_t type, StringView name) noexcept
{
	if (type == Scene::TypeData.Hash)
	{
		// シーン切り替えでリソースが消去されるため
		DetailsWidget::ClearSelectObject();
		EditorHelper::Get().FlushCommandList();

		m_world->ChangeScene(name);
	}
}

void ResourceWidget::RegisterResourceColor() noexcept
{
	m_color[Scene::TypeData.Hash    ] = ImVec4(0.6f, 0.4f, 0.0f, 1.0f); // 茶
	m_color[Model::TypeData.Hash    ] = ImVec4(0.0f, 0.3f, 0.7f, 1.0f); // 青
	m_color[Mesh::TypeData.Hash     ] = ImVec4(0.5f, 0.6f, 1.0f, 0.6f); // 水
	m_color[Material::TypeData.Hash ] = ImVec4(0.1f, 1.0f, 0.1f, 0.6f); // 緑
	m_color[Texture::TypeData.Hash  ] = ImVec4(0.9f, 0.5f, 0.0f, 0.6f); // オレンジ
	//m_color[AudioClip::TypeData.Hash] = ImVec4(0.9f, 0.5f, 0.0f, 0.6f); //
}

bool ResourceWidget::IsTimeOut() noexcept
{
	constexpr auto timeOut = 300;
	return m_stopwatch.GetRap(Milli) >= timeOut;
}

bool ResourceWidget::IsShowAllResourceType() noexcept
{
	return m_tag == TagType_ALL;
}

bool ResourceWidget::IsShowCreateWindow() noexcept
{
	return !!m_resourceCreateFunc;
}