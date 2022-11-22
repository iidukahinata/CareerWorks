/**
* @file	   ProfilerWidget.cpp
* @brief
*
* @date	   2022/10/21 2022”N“x‰”Å
*/


#include "ProfilerWidget.h"
#include "ThirdParty/imgui/implot.h"
#include "ThirdParty/imgui/imgui_time_line.h"

void ProfilerWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0,  20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(220, 60), ImGuiCond_FirstUseEver);

	ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
	ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::SameLine(250);
	if (ImGui::Button("TimeLine On/Off", ImVec2(150, 20)))
	{
		ImTimeLine::ShowTimeLine(!ImTimeLine::ShowTimeLine());
	}

	if (ImTimeLine::ShowTimeLine())
	{
		ShowTimeLine();
	}

	ImGui::End();
}

void ProfilerWidget::ShowTimeLine() noexcept
{
	auto mainThread = "Main Thread";
	auto taskThread = "Task Thread";
	auto renderingThread = "Rendering Thread";
	auto size = ImGui::GetWindowSize();

	if (size.x > 136) size.x -= 135;
	else mainThread = taskThread = renderingThread = "";

	if (size.y > 100) { size.y -= 70; size.y /= 3.f; }
	else { size.y = 0.0f; mainThread = taskThread = renderingThread = ""; }

	ImTimeLine::DrawTimeLine(ImTimeLine::MainThread, mainThread, nullptr, 0, 0.f, 20.0f, size);
	ImTimeLine::DrawTimeLine(ImTimeLine::TaskThread, taskThread, nullptr, 0, 0.f, 20.0f, size);
	ImTimeLine::DrawTimeLine(ImTimeLine::RenderingThread, renderingThread, nullptr, 0, 0.f, 20.0f, size);
}