/**
* @file	   AssetsWidget.cpp
* @brief
*
* @date	   2022/10/02 2022�N�x����
*/


#include "AssetsWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Shader.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"

void AssetsWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	NavigateToDirectory("Data/asset");
}

void AssetsWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(420, 550), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(680, 315), ImGuiCond_Once);

	ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ShowResourceMenu();
	ShowResourceList();
	ShowResourceHelper();

	ImGui::End();
}

void AssetsWidget::ShowResourceMenu() noexcept
{
	// tag �����p Filter ��`
	if (ImGui::TreeNode("Filter"))
	{
		auto isChangeTag = false;

		ImGui::Columns(3, nullptr, false);

		isChangeTag |= ImGui::RadioButton("ALL", &m_tag, TagType_ALL);
		ImGui::NextColumn();

#define CREATE_RADIO_BUTTON_FROM_TYPE(CLASS)												  \
isChangeTag |= ImGui::RadioButton(CLASS::TypeData.Name.data(), &m_tag, CLASS::TypeData.Hash); \
ImGui::NextColumn();

		// �^�O�w��p�{�^���̍쐬
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

	// �����񌟍��p Filter ��`
	m_filter.Draw("Search", 200.0f);

	ImGui::Separator();
	ImGui::Text("asset"); ImGui::SameLine();

	// �t�@�C�����̕\���ƃo�b�N�{�^���̐���
	auto numTree = m_directoryTree.size();
	for (int i = 0; i < numTree; ++i)
	{
		if (ImGui::SmallButton(String(">##" + std::to_string(i)).c_str()))
		{
			for (int index = numTree - i; index > 0; --index)
			{
				CurrentDirectoryToParent();
			}
			break;
		}

		ImGui::SameLine();

		ImGui::Text(m_directoryTree[i].c_str()); ImGui::SameLine();
	}

	ImGui::Text("");
}

void AssetsWidget::ShowResourceList() noexcept
{
	const auto childWidth = ImGui::GetWindowContentRegionWidth();
	const auto childHeight = ImGui::GetWindowHeight() - 82;
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(childWidth, childHeight), true, ImGuiWindowFlags_NoScrollbar);

	const auto columns = static_cast<int>(std::round(max(ImGui::GetWindowContentRegionWidth() / 110.f, 1.f)));
	ImGui::Columns(columns, nullptr, false);

	if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
	{
		m_selectResoruce = nullptr;
	}

	// �ʏ�� CurrentDirectory �����̃��\�[�X�̂ݗ񋓂��邪�A
	// filter �Ń��\�[�X�������s���ꍇ�� File �\�����l�������ɗ�
	if (m_filter.IsActive())
	{
		ShowResourceListByName();
	}
	else
	{
		ShowCurrentDirectoryResourceList();
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
	}

	ImGui::Columns(1);
	ImGui::EndChild();
}

void AssetsWidget::ShowCurrentDirectoryResourceList() noexcept
{
	for (const auto& entries : m_directoryEntries)
	{
		DrawThumbnail(entries.type, entries.name);
	}
}

void AssetsWidget::ShowResourceListByName() noexcept
{
	for (const auto& directory : FileSystem::GetDirectorysRecursiveDirectory(ASSET_DATA_DIRECTORY))
	{
		auto&& name = FileSystem::GetFilePath(directory);

		// �w�蕶������܂� Resource �̂ݗ�
		if (!m_filter.PassFilter(name.c_str()))
		{
			continue;
		}

		DrawThumbnail(Icon_Folder, name);
	}

	// �\������� Type ������
	Vector<uint32_t> showTypeList;
	for (const auto type : m_resourceManager->GetResourceTypes())
	{
		if (m_tag == TagType_ALL || m_tag == type)
		{
			showTypeList.emplace_back(type);
		}
	}

	// �S���\�[�X�\�����[�v
	for (const auto type : showTypeList)
	{
		const auto iconType = EditorHelper::Get().GetIconTypeFromResourceType(type);

		for (const auto& resourceInfo : m_resourceManager->GetResourceDataListByType(type))
		{
			const auto& assetName = resourceInfo.first;

			// �w�蕶������܂� Resource �̂ݗ�
			if (!m_filter.PassFilter(assetName.c_str()))
			{
				continue;
			}

			DrawThumbnail(iconType, assetName);
		}
	}
}

void AssetsWidget::ShowResourceHelper() noexcept
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseClicked(1))
	{
		ImGui::OpenPopup("Assets Helper");
		m_clickedMausePos = ImGui::GetMousePos();
	}

	ImGui::SetNextWindowPos(m_clickedMausePos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(150, 200), ImGuiCond_Always);

	bool openCreateFolder = false;
	bool OpenCreateResource = false;
	const auto buttonSize = ImVec2(135, 20);

	// Draw Assets Helper
	if(ImGui::BeginPopup("Assets Helper"))
	{
		if (ImGui::Button("Create Folder", buttonSize))
		{
			openCreateFolder = true;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Scene", buttonSize))
		{
			OpenCreateResource = true;
			m_resourceCreateFunc = [](StringView name) { return Scene::Create(name); };
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Create Material", buttonSize))
		{
			OpenCreateResource = true;
			m_resourceCreateFunc = [](StringView name) { return Material::Create(name); };
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// Open Popup
	if (openCreateFolder)
	{
		ImGui::OpenPopup("Create Folder");
	}
	if (OpenCreateResource)
	{
		ImGui::OpenPopup("Create Resource");
	}

	// Draw Create Folder
	if (ImGui::BeginPopupModal("Create Folder"))
	{
		ShowCreateWindow([this](StringView name) {

			FileSystem::CreateDirectory(m_currentDirectory + "/" + name.data());

		});
	}

	// Draw Create Resource
	if (ImGui::BeginPopupModal("Create Resource"))
	{
		ShowCreateWindow([this](StringView name) {

			auto resource = m_resourceCreateFunc(m_currentDirectory + "/" + name.data());

			// ���\�[�X�̐����������ړI�̂��߉��
			UnloadResource(resource);
		});
	}
}

bool AssetsWidget::ShowCreateWindow(std::function<void(StringView)> createFunc) noexcept
{
	ImGui::Text("Object Name"); ImGui::SameLine();

	char name[128] = "";
	auto isCreate = ImGui::InputTextWithHint("", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // ���s
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

	return (isCreate || isCancel);
}

void AssetsWidget::ChackClickedCommand(IconType type, StringView name) noexcept
{
	if (!ImGui::IsItemHovered())
	{
		return;
	}

	// �h���b�O�A���h�h���b�v�̂��߂� Down ������
	if (ImGui::IsMouseClicked(0) && m_selectResourceName != name)
	{
		m_stopwatch.Start();
		ClickResource(type, name);
	}

	else if (ImGui::IsMouseDoubleClicked(0))
	{
		DoubleClickResource(type, name);
	}

	// ���͏I�����̑I���� Details �ɕ\��������悤�ɂ���
	else if (ImGui::IsMouseReleased(0) && m_selectResoruce)
	{
		DetailsWidget::SelectResource(m_selectResoruce);
	}
}

void AssetsWidget::DrawThumbnail(IconType type, StringView name) noexcept
{
	// Image Button
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.5, 0.5, 0.8));

	const auto iconTexture = EditorHelper::Get().GetIconTexture(type);
	EditorHelper::Get().AddImageButton(iconTexture->GetData(), ImVec2(90, 90));

	ImGui::PopStyleColor();

	// Handle Clicked
	ChackClickedCommand(type, name);

	// Label
	auto&& label = ConvertToJapanese(name);
	ImGui::Text(label.c_str());

	ImGui::NextColumn();
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
			// �V�[���؂�ւ��Ń��\�[�X����������邽��
			DetailsWidget::ClearSelectObject();
			EditorHelper::Get().FlushCommandList();

			m_world->ChangeScene(name);
		}
	}
}

void AssetsWidget::NavigateToDirectory(StringView path) noexcept
{
	m_currentDirectory = path;

	m_directoryEntries.clear();
	m_directoryEntries.shrink_to_fit();

	// �f�B���N�g����
	for (const auto& directory : FileSystem::GetDirectorysFromDirectory(path))
	{
		m_directoryEntries.emplace_back(Thumbnail(FileSystem::GetFilePath(directory), Icon_Folder));
	}

	Vector<std::pair<String, ResourceData*>> resourceDatas;
	m_resourceManager->GetResourceDataListFromDirectory(path, resourceDatas);

	// ���\�[�X��
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