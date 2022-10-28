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
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"

void MainMenuBarWidget::PostInitialize()
{
    m_world = GetContext()->GetSubsystem<World>();
    ASSERT(m_world);

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

        if (ImGui::BeginMenu("View"))
        {
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