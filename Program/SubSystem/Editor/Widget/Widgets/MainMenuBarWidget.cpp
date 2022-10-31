/**
* @file	   MainMenuBarWidget.cpp
* @brief
*
* @date	   2022/10/21 2022年度初版
*/


#include "MainMenuBarWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Texture.h"

void MainMenuBarWidget::PostInitialize()
{
    m_world = GetContext()->GetSubsystem<World>();
    ASSERT(m_world);

    m_renderer = GetContext()->GetSubsystem<Renderer>();
    ASSERT(m_renderer);

    m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
    ASSERT(m_resourceManager);
}

void MainMenuBarWidget::Draw()
{
    static auto saveScene = false;
    static auto saveAsScene = false;
    auto newScene = false;

    // Input Handle
    newScene    |= ImGui::IsKeyDown(ImGuiKey_ModCtrl) &&  ImGui::IsKeyReleased(ImGuiKey_N);
    saveScene   |= ImGui::IsKeyDown(ImGuiKey_ModCtrl) && !ImGui::IsKeyDown(ImGuiKey_ModShift) && ImGui::IsKeyReleased(ImGuiKey_S);
    saveAsScene |= ImGui::IsKeyDown(ImGuiKey_ModCtrl) &&  ImGui::IsKeyDown(ImGuiKey_ModShift) && ImGui::IsKeyReleased(ImGuiKey_S);

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.20f, 0.20f, 0.10f));

	if (ImGui::BeginMainMenuBar())
	{
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) 
            {
                newScene = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) 
            {
                saveScene = true;
            }
            if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) 
            {
                saveAsScene = true;
            }
            if (ImGui::MenuItem("Settings"))
            {
                m_openSettginsWindow = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            {
                EditorHelper::Get().UndoCommand();
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y"))
            {
                EditorHelper::Get().RedoCommand();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("Import"))
            {
                system("explorer");
            }
            ImGui::EndMenu();
        }

        if (Window::Get().IsFullscreen())
        {
            ShowWindowMenu();
        }

        ImGui::EndMainMenuBar();
	}

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    // Handle Command
    if (newScene)
    {
        ImGui::OpenPopup("New Scene");
    }
    if (saveScene)
    {
        m_world->GetCurrentScene()->Update();
        LOG("Successfully Saved");
        saveScene = false;
    }
    if (saveAsScene)
    {
        ImGui::OpenPopup("Save as");
        saveAsScene = false;
    }

    ShowNewSceneModal();
    ShowSaveAsModal();

    ShowSettingsWindow();
}

void MainMenuBarWidget::Serialized(FileStream* file) const
{
    if (!m_renderer)
    {
        return;
    }

    const auto skybox = m_renderer->GetSkyBox();
    if (auto texture = skybox->GetTexture())
    {
        file->Write(texture->GetFilePath());
    }
    else
    {
        file->Write(String(""));
    }
}

void MainMenuBarWidget::Deserialized(FileStream* file)
{
    String texturePath;
    file->Read(&texturePath);
    
    if (!texturePath.empty())
    {
        if (auto resourceData = m_resourceManager->GetResourceData(texturePath))
        {
            m_renderer->GetSkyBox()->SetTexture(LoadResource<Texture>(resourceData));
        }
    }
}

void MainMenuBarWidget::ShowSettingsWindow() noexcept
{
    constexpr auto offsetPos = 130;

    if (!m_openSettginsWindow)
    {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(500, 250), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(390, 480), ImGuiCond_FirstUseEver);

    ImGui::Begin("Settings", &m_openSettginsWindow, ImGuiWindowFlags_NoCollapse);

    if (ImGui::CollapsingHeader("SubSystem", ImGuiTreeNodeFlags_DefaultOpen))
    {
        constexpr auto rendererTypeCombo = "Forward\0Deferred\0\0";
        constexpr auto inputTypeCombo    = "Dirext\0\0";
        constexpr auto audioTypeCombo    = "FMOD\0\0";

        auto rendererType = Config::GetRendererType();
        auto inputType    = Config::GetInputType();
        auto audioType    = Config::GetAudioType();

        ImGui::Text("RendererType"); ImGui::SameLine(offsetPos);
        auto inputRenderer = ImGui::Combo("##RendererType", (int*)(&rendererType), rendererTypeCombo);

        ImGui::Text("InputType"); ImGui::SameLine(offsetPos);
        auto inputInput = ImGui::Combo("##InputType", (int*)(&inputType), inputTypeCombo);

        ImGui::Text("AudioType"); ImGui::SameLine(offsetPos);
        auto inputAudio = ImGui::Combo("##AudioType", (int*)(&audioType), audioTypeCombo);

        //if (inputRenderer) Config::RegisterRendererSystem(rendererType);
        //if (inputInput)    Config::RegisterInputSystem(inputType);
        //if (inputAudio)    Config::RegisterAudioSystem(audioType);
    }

    if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto skybox  = m_renderer->GetSkyBox();
        auto texture = skybox->GetTexture();
        ShowTexture("sky box", texture, [skybox](auto data) { skybox->SetTexture(data); });
    }

    ImGui::End();
}

void MainMenuBarWidget::ShowNewSceneModal() noexcept
{
    const auto center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(ImVec2(center.x - 150, center.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);

    if (ImGui::BeginPopupModal("New Scene"))
    {
        ImGui::Text("Name"); ImGui::SameLine();

        char name[128] = "Untitled";
        auto isCreate = ImGui::InputTextWithHint("##NewSceneName", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行
        auto isCancel = ImGui::Button("Cancel");
        isCreate |= ImGui::IsKeyDown(ImGuiKey_Enter);

        if (isCreate)
        {
            if (auto scene = Scene::Create(name))
            {
                auto camera = m_world->CreateGameObject(scene);
                camera->SetName("Main Camera");
                camera->AddComponent("Camera");
                camera->AddComponent("AudioListener");

                auto DirectionalLight = m_world->CreateGameObject(scene);
                DirectionalLight->SetName("Directional Light");
                DirectionalLight->AddComponent("Light");

                scene->Update();

                m_resourceManager->Unload<Scene>(name);

                // シーン切り替えでリソースが消去されるため
                DetailsWidget::ClearSelectObject();
                EditorHelper::Get().FlushCommandList();

                m_world->ChangeScene(name);
            }
        }

        if (isCreate || isCancel)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void MainMenuBarWidget::ShowSaveAsModal() noexcept
{
    const auto center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(ImVec2(center.x - 150, center.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);

    if (ImGui::BeginPopupModal("Save as"))
    {
        ImGui::Text("Name"); ImGui::SameLine();

        char name[128] = "";
        const auto isCreate = ImGui::InputTextWithHint("##SaveSceneName", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行
        const auto isCancel = ImGui::Button("Cancel");

        if (isCreate)
        {
            m_world->GetCurrentScene()->SaveAs(name);

            // シーン切り替えでリソースが消去されるため
            DetailsWidget::ClearSelectObject();
            EditorHelper::Get().FlushCommandList();

            m_world->ChangeScene(name);

            LOG("Successfully Saved");
        }

        if (isCreate || isCancel)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void MainMenuBarWidget::ShowWindowMenu() noexcept
{
    auto width = ImGui::GetWindowWidth();
    static auto minmize = ConvertToJapanese("ー");
    static auto destroy = ConvertToJapanese("X");

    // show minmize button
    ImGui::SameLine(width - 69);
    if (ImGui::Button(minmize.c_str(), ImVec2(30, 20)))
    {
        ShowWindow(Window::Get().GetHandle(), SW_MINIMIZE);
        CloseWindow(Window::Get().GetHandle());
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    auto min = ImGui::GetItemRectMin();
    auto max = ImGui::GetItemRectMax();
    draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 200), 0, 0, 2);

    // show destroy button
    ImGui::SameLine(width - 39);
    if (ImGui::Button(destroy.c_str(), ImVec2(30, 20)))
    {
        DestroyWindow(Window::Get().GetHandle());
    }

    min = ImGui::GetItemRectMin();
    max = ImGui::GetItemRectMax();
    draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 200), 0, 0, 2);
}

void MainMenuBarWidget::ShowTexture(StringView label, Texture* texture, std::function<void(Texture*)> collBack) noexcept
{
    constexpr auto offsetPos = 130;
    auto texturePath = texture ? ConvertToJapanese(texture->GetFilePath().c_str()) : String();

    ImGui::Text(label.data()); ImGui::SameLine(offsetPos);

    constexpr auto itemWidth = 228;
    ImGui::PushItemWidth(itemWidth);
    ImGui::InputText((String("##") + label.data()).c_str(), texturePath.data(), texturePath.size());
    ImGui::PopItemWidth();

    // ドラッグアンドドロップでの Texture 切り替え
    const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    if (ImGui::IsMouseReleased(0) && hoverd)
    {
        if (auto catchTexture = CatchDragObject<Texture>())
        {
            collBack(catchTexture);
        }
    }

    ShowDragDropHelper<Texture>(hoverd, 156, 0, 31);

    ImGui::SameLine(offsetPos + itemWidth + 5);

    // 検索での Texture 切り替え
    OpenResourceHelper(label.data());
    if (auto resourceData = ShowSearchResourceHelper<Texture>())
    {
        if (auto catchTexture = LoadResource<Texture>(resourceData))
        {
            collBack(catchTexture);
        }
    }
}

void MainMenuBarWidget::ShowDragDropHelper(uint32_t selctType, bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    auto min = ImGui::GetItemRectMin();
    auto max = ImGui::GetItemRectMax();

    // 少し大きめの枠にする
    min.x -= 3; min.y -= 3;
    max.x += 3; max.y += 3;

    if (DragDrop::Get().HasResource())
    {
        constexpr int selectAlpha = 255;
        constexpr int noSelectAlpha = 200;
        int thickness = 3;

        const auto resrouceData = std::any_cast<ResourceData*>(DragDrop::Get().GetDragObject());
        const auto type = resrouceData->m_resourcePath.m_type;

        // 同じリソースタイプのみ受付
        if (type == selctType)
        {
            if (hovered)
            {
                // pop up 的な動きをさせる
                min.x -= 2; min.y -= 2;
                max.x += 2; max.y += 2;
                thickness = 4;
            }

            auto alpha = hovered ? selectAlpha : noSelectAlpha;
            draw_list->AddRect(min, max, IM_COL32(r, g, b, alpha), 0, 0, thickness);
        }
        else
        {
            // 赤い禁止するような UI 表示
            draw_list->AddLine(min, max, IM_COL32(200, 0, 0, 180), thickness);
            draw_list->AddRect(min, max, IM_COL32(200, 0, 0, 210), 0, 0, 1);
        }
    }
    else
    {
        // 細目の白いライン表示
        draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 120), 0, 0, 1);
    }
}

IResource* MainMenuBarWidget::CatchDragResourceObject(uint32_t selectType) const noexcept
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

    return LoadResource(resourceData);
}

ResourceData* MainMenuBarWidget::ShowSearchResourceHelper(uint32_t selectType) noexcept
{
    ResourceData* resourceData = nullptr;

    if (ImGui::BeginPopup(m_searchResourceLavel.c_str()))
    {
        // 文字列検索用 Filter 定義
        m_searchResourceFilter.Draw("Search", 220.0f);

        ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(290, 300), true, ImGuiWindowFlags_NoScrollbar);

        constexpr auto columns = 2;
        ImGui::Columns(columns, nullptr, false);

        const auto& resourceDataMap = m_resourceManager->GetResourceDataListByType(selectType);

        if (!resourceDataMap.empty())
        {
            for (const auto& resourceInfo : resourceDataMap)
            {
                const auto& assetName = resourceInfo.first;
                const auto& assetData = resourceInfo.second;

                // 指定文字列を含む Resource のみ列挙
                if (!m_searchResourceFilter.PassFilter(assetName.c_str()))
                {
                    continue;
                }

                auto&& label = ConvertToJapanese(assetName);
                if (ImGui::Button(label.c_str(), ImVec2(130, 120)))
                {
                    resourceData = m_resourceManager->GetResourceData(selectType, assetName);
                    ImGui::CloseCurrentPopup();
                    break;
                }
                ImGui::NextColumn();
            }
        }

        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::EndPopup();
    }

    return resourceData;
}

bool MainMenuBarWidget::OpenResourceHelper(StringView lavel) noexcept
{
    const auto buttonLavel = String("*##") + lavel.data();
    m_searchResourceLavel = String("SearchResourceHelper") + lavel.data();

    if (ImGui::Button(buttonLavel.c_str(), ImVec2(20, 20)))
    {
        ImGui::OpenPopup(m_searchResourceLavel.c_str());
        return true;
    }
    return false;
}

IResource* MainMenuBarWidget::LoadResource(ResourceData* resourceData) const noexcept
{
    auto resourceHandle = m_resourceManager->Load(resourceData);
    resourceHandle->WaitForLoadComplete();

    return resourceHandle->GetResource();
}