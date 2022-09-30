/**
* @file	   ProfilerWidget.cpp
* @brief
*
* @date	   2022/09/06 2022îNìxèâî≈
*/


#include "ProfilerWidget.h"

void ProfilerWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0,  20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(220, 60), ImGuiCond_Once);

	ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}