/**
* @file	   ConsoleWidget.h
* @brief
*
* @date	   2022/10/21 2022年度初版
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

	// * Log 情報保持クラス
	EngineLog m_logInfo;

	// * filter 検索用
	ImGuiTextFilter m_logFilter;

	// * TextColor List
	Array<ImVec4, LogType_Max> m_logColor;
};