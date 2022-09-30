/**
* @file	   MainMenuBarWidget.cpp
* @brief
*
* @date	   2022/09/29 2022îNìxèâî≈
*/


#include "MainMenuBarWidget.h"

void MainMenuBarWidget::Draw()
{
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.00f));

	if (ImGui::BeginMainMenuBar())
	{
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
	}

    ImGui::PopStyleColor();
}