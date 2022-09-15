/**
* @file	   LogWidget.h
* @brief
*
* @date	   2022/09/09 2022�N�x����
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

class LogWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void Initialize() override;
	void Draw() override;

private:

	EngineLog m_logInfo;

	ImGuiTextFilter m_logFilter;

	Array<ImVec4, LogType_Max> m_logColor;
};