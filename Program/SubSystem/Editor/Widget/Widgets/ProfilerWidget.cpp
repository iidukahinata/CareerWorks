/**
* @file	   ProfilerWidget.cpp
* @brief
*
* @date	   2022/10/21 2022îNìxèâî≈
*/


#include "ProfilerWidget.h"

void ProfilerWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0,  20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(220, 60), ImGuiCond_FirstUseEver);

	ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}