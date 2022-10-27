/**
* @file	   MainMenuBarWidget.cpp
* @brief
*
* @date	   2022/10/21 2022年度初版
*/


#include "MainMenuBarWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
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

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Profiler")) 
            {

            }
            if (ImGui::MenuItem("Settings")) 
            {

            }
            if (ImGui::MenuItem("Renderer")) 
            {

            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
	}

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
        const auto isCreate = ImGui::InputTextWithHint("##NewSceneName", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // 改行
        const auto isCancel = ImGui::Button("Cancel");

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

                // シーン切り替えでリソースが消去されるため
                DetailsWidget::ClearSelectObject();
                EditorHelper::Get().FlushCommandList();

                m_world->ChangeScene(name);

                scene->Update();
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