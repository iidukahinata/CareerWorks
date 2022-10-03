/**
* @file	   MainMenuBarWidget.cpp
* @brief
*
* @date	   2022/09/30 2022”N“x‰”Å
*/


#include "MainMenuBarWidget.h"
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

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.00f));

	if (ImGui::BeginMainMenuBar())
	{
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) 
            {

            }
            if (ImGui::MenuItem("Open...", "Ctrl+O")) 
            {

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
    if (saveScene)
    {
        m_world->GetCurrentScene()->Update();
        saveScene = false;
    }
    if (saveAsScene)
    {
        ImGui::OpenPopup("Save as");
        saveAsScene = false;
    }

    ShowSaveAsModal();
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
        const auto isCreate = ImGui::InputTextWithHint("", "none", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue); ImGui::Text(""); // ‰üs
        const auto isCancel = ImGui::Button("Cancel");

        if (isCreate)
        {
            m_world->GetCurrentScene()->Clone(name);
        }

        if (isCreate || isCancel)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}