/**
* @file	   MainMenuBarWidget.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "MainMenuBarWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Physics/IPhysics.h"
#include "SubSystem/Renderer/IRenderer.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

void MainMenuBarWidget::PostInitialize()
{
    m_world = GetContext()->GetSubsystem<World>();
    ASSERT(m_world);

    m_renderer = GetContext()->GetSubsystem<IRenderer>();
    ASSERT(m_renderer);

    m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
    ASSERT(m_resourceManager);

    m_physics = GetContext()->GetSubsystem<IPhysics>();
    ASSERT(m_physics);
}

void MainMenuBarWidget::Draw()
{
    ShowMainMenuBar();

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
    if (auto material = skybox->GetMaterial())
    {
        file->Write(material->GetFilePath());
    }
    else
    {
        file->Write(String(""));
    }
}

void MainMenuBarWidget::Deserialized(FileStream* file)
{
    String materialPath;
    file->Read(&materialPath);
    
    if (!materialPath.empty())
    {
        if (auto resourceData = m_resourceManager->GetResourceData(materialPath))
        {
            m_renderer->GetSkyBox()->SetMaterial(LoadResource<Material>(resourceData));
        }
    }
}

void MainMenuBarWidget::ShowMainMenuBar() noexcept
{
    auto saveScene = false;
    auto saveAsScene = false;
    auto newScene = false;

    // Input Handle
    newScene |= ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyReleased(ImGuiKey_N);
    saveScene |= ImGui::IsKeyDown(ImGuiKey_ModCtrl) && !ImGui::IsKeyDown(ImGuiKey_ModShift) && ImGui::IsKeyReleased(ImGuiKey_S);
    saveAsScene |= ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyDown(ImGuiKey_ModShift) && ImGui::IsKeyReleased(ImGuiKey_S);

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

            ImGui::Separator();
            if (ImGui::MenuItem("Settings"))
            {
                m_openSettginsWindow = true;
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
    }
    if (saveAsScene)
    {
        ImGui::OpenPopup("Save as");
    }
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
                camera->AddComponent("ICamera");
                camera->AddComponent("IAudioListener");

                auto DirectionalLight = m_world->CreateGameObject(scene);
                DirectionalLight->SetName("Directional Light");
                DirectionalLight->AddComponent("ILight");

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
        NotifyEvent<CommandWindowEvent>(SW_MINIMIZE);
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    auto min = ImGui::GetItemRectMin();
    auto max = ImGui::GetItemRectMax();
    draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 200), 0, 0, 2);

    // show destroy button
    ImGui::SameLine(width - 39);
    if (ImGui::Button(destroy.c_str(), ImVec2(30, 20)))
    {
        NotifyEvent<DestroyWindowEvent>();
    }

    min = ImGui::GetItemRectMin();
    max = ImGui::GetItemRectMax();
    draw_list->AddRect(min, max, IM_COL32(100, 100, 100, 200), 0, 0, 2);
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

    ShowSubsystemSettings();
    ShowRendererSettings();
    ShowPhysicsSettings();

    ImGui::End();
}

void MainMenuBarWidget::ShowSubsystemSettings() noexcept
{
    constexpr auto offsetPos = 130;

    if (ImGui::CollapsingHeader("Subsystem", ImGuiTreeNodeFlags_DefaultOpen))
    {
        constexpr auto rendererTypeCombo = "Forward\0Deferred\0\0";
        constexpr auto inputTypeCombo = "Dirext\0\0";
        constexpr auto audioTypeCombo = "FMOD\0\0";
        constexpr auto physicsTypeCombo = "PhysX\0\0";

        auto rendererType = Config::GetRendererType();
        auto inputType    = Config::GetInputType();
        auto audioType    = Config::GetAudioType();
        auto physicsType  = Config::GetPhysicsType();

        ImGui::Text("RendererType"); ImGui::SameLine(offsetPos);
        auto inputRenderer = ImGui::Combo("##RendererType", (int*)(&rendererType), rendererTypeCombo);

        ImGui::Text("InputType"); ImGui::SameLine(offsetPos);
        auto inputInput = ImGui::Combo("##InputType", (int*)(&inputType), inputTypeCombo);

        ImGui::Text("AudioType"); ImGui::SameLine(offsetPos);
        auto inputAudio = ImGui::Combo("##AudioType", (int*)(&audioType), audioTypeCombo);

        ImGui::Text("PhysicsType"); ImGui::SameLine(offsetPos);
        auto inputPhysics = ImGui::Combo("##PhysicsType", (int*)(&physicsType), physicsTypeCombo);

        if (inputRenderer) Config::RegisterRendererSystem(rendererType);
        if (inputInput)    Config::RegisterInputSystem(inputType);
        if (inputAudio)    Config::RegisterAudioSystem(audioType);
        if (inputPhysics)  Config::RegisterPhysicsSystem(physicsType);

        // change system
        if (inputRenderer || inputInput || inputAudio || inputPhysics)
        {
            ImGui::OpenPopup("Change System");
        }

        ShowRestartSystemModal();
    }

    ImGui::Separator();
}

void MainMenuBarWidget::ShowPhysicsSettings() noexcept
{
    constexpr auto offsetPos = 130;

    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto gravity = m_physics->GetGravity();

        ImGui::Text("gravity"); ImGui::SameLine(offsetPos);
        if (ImGui::InputFloat3("##gravity", &gravity.x))
        {
            m_physics->SetGravity(gravity);
        }
    }

    ImGui::Separator();
}

void MainMenuBarWidget::ShowRendererSettings() noexcept
{
    if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto skybox = m_renderer->GetSkyBox();
        auto material = skybox->GetMaterial();
        ShowMaterial("sky box", material, [skybox](auto data) { skybox->SetMaterial(data); });
    }

    ImGui::Separator();
}

void MainMenuBarWidget::ShowRestartSystemModal() noexcept
{
    const auto center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(ImVec2(center.x - 150, center.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(470, 115), ImGuiCond_Once);

    if (ImGui::BeginPopupModal("Change System"))
    {
        auto text1 = ConvertToJapanese("System を変更するためにプログラムを再起動しますか？");
        auto text2 = ConvertToJapanese("シーンを変更している場合、保存することをおすすめします。");

        ImGui::Text(text1.c_str());
        ImGui::Text(text2.c_str());

        ImGui::Text("");
        auto isRestart = ImGui::Button("Restart"); ImGui::SameLine();
        auto isRestartAndSave = ImGui::Button("Restart And Save"); ImGui::SameLine();
        auto isCancel = ImGui::Button("Cancel");

        if (isRestart || isRestartAndSave)
        {
            if (auto scene = m_world->GetCurrentScene())
            {
                if (isRestartAndSave)
                    scene->Update();
            }

            NotifyEvent<DestroyWindowEvent>();
            system("start CareerWorks.exe");
        }

        if (isRestart || isRestartAndSave || isCancel)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void MainMenuBarWidget::ShowMaterial(StringView label, Material* material, std::function<void(Material*)> collBack) noexcept
{
    constexpr auto offsetPos = 130;
    auto materialPath = material ? ConvertToJapanese(material->GetFilePath().c_str()) : String();

    ImGui::Text(label.data()); ImGui::SameLine(offsetPos);

    constexpr auto itemWidth = 228;
    ImGui::PushItemWidth(itemWidth);
    ImGui::InputText((String("##") + label.data()).c_str(), materialPath.data(), materialPath.size());
    ImGui::PopItemWidth();

    // ドラッグアンドドロップでの Texture 切り替え
    const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    if (ImGui::IsMouseReleased(0) && hoverd)
    {
        if (auto catchMaterial = CatchDragObject<Material>())
        {
            collBack(catchMaterial);
        }
    }

    ShowDragDropHelper<Material>(hoverd, 25, 255, 25);

    ImGui::SameLine(offsetPos + itemWidth + 5);

    // 検索での Texture 切り替え
    OpenResourceHelper(label.data());
    if (auto resourceData = ShowSearchResourceHelper<Material>())
    {
        if (auto catchMaterial = LoadResource<Material>(resourceData))
        {
            collBack(catchMaterial);
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
    const auto buttonLavel = "*##" + lavel;
    m_searchResourceLavel = "SearchResourceHelper" + lavel;

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