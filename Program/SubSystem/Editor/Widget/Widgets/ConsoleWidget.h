/**
* @file	   ConsoleWidget.h
* @brief
*
* @date	   2022/09/27 2022îNìxèâî≈
*/
#pragma once


#include "../Widget.h"

struct EngineLog
{
private:

	struct LogInfo
	{
		String str;
		uint32_t errorLevel;
	};

	Vector<LogInfo> m_logList;

public:

	void AddLog(StringView log, uint32_t errorLevel) noexcept;
	const Vector<LogInfo>& GetList() const noexcept;
	void Clear() noexcept;
};

class ConsoleWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

private:

	EngineLog m_logInfo;

	ImGuiTextFilter m_logFilter;

	Array<ImVec4, LogType_Max> m_logColor;
};