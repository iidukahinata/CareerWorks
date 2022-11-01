/**
* @file    WindowEvents.h
* @brief
*
* @date	   2022/11/01 2022îNìxèâî≈
*/
#pragma once


class DestroyWindowEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(DestroyWindowEvent)
public:

	DestroyWindowEvent();
	std::any GetData() override;
};

class CommandWindowEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CommandWindowEvent)
public:

	CommandWindowEvent(int cmd);
	std::any GetData() override;

private:

	int m_cmd = 0;
};