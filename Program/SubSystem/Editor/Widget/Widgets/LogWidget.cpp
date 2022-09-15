/**
* @file	   LogWidget.cpp
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
*/


#include "LogWidget.h"
#include "SubSystem/Log/DebugLog.h"

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

void LogWidget::Initialize()
{
	DebugLog::Get().SetLogInfo(&m_logInfo);

	m_logColor[LogType_Info] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
	m_logColor[LogType_Error] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void LogWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(1100, 630), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(435, 230), ImGuiCond_Once);

	ImGui::Begin("Log Info", nullptr, ImGuiWindowFlags_NoCollapse);

	m_logFilter.Draw("Filter", 150.0f); ImGui::SameLine();

	if (ImGui::Button("clear"))
	{
		m_logInfo.Clear();
	}

	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(405, 170), true);

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