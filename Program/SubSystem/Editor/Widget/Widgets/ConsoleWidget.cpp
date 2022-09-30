/**
* @file	   ConsoleWidget.cpp
* @brief
*
* @date	   2022/09/27 2022îNìxèâî≈
*/


#include "ConsoleWidget.h"
#include "SubSystem/Log/DebugLog.h"

#include "SubSystem/Resource/Resources/3DModel/Texture.h"

void EngineLog::AddLog(StringView log, uint32_t errorLevel) noexcept
{
	LogInfo info;
	info.str = log;
	info.errorLevel = errorLevel;
	m_logList.emplace_back(info);
}

const Vector<EngineLog::LogInfo>& EngineLog::GetList() const noexcept
{
	return m_logList;
}

void EngineLog::Clear() noexcept
{
	m_logList.clear();
}

void ConsoleWidget::Initialize()
{
	DebugLog::Get().SetLogInfo(&m_logInfo);

	m_logColor[LogType_Info] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	m_logColor[LogType_Error] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void ConsoleWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 550), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(420, 315), ImGuiCond_Once);

	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::Text("Filter"); ImGui::SameLine();
	m_logFilter.Draw("##Filter", 150.0f); ImGui::SameLine();

	if (ImGui::Button("Clear"))
	{
		m_logInfo.Clear();
	}

	auto childWidth = ImGui::GetWindowWidth() - 15;
	auto childHeight = ImGui::GetWindowHeight() - 60;
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(childWidth, childHeight), true);

	for (const auto& logList = m_logInfo.GetList(); const auto& log : logList)
	{
		if (!m_logFilter.PassFilter(log.str.c_str()))
			continue;

		ImGui::PushStyleColor(ImGuiCol_Text, m_logColor[log.errorLevel]);

		auto logText = ConvertToJapanese(log.str);
		ImGui::Text(logText.c_str());

		ImGui::PopStyleColor();
	}

	ImGui::EndChild();
	ImGui::End();
}