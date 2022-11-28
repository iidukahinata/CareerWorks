/**
* @file    InputEvents.h
* @brief   入力関係のイベントクラス定義
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "InputHelper.h"

class KeyPressedEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(KeyPressedEvent, IEvent)
public:

	KeyPressedEvent(Button::KeyAndMouse key);
	std::any GetData() override;

private:

	Button::KeyAndMouse m_key;
};

class KeyReleasedEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(KeyReleasedEvent, IEvent)
public:

	KeyReleasedEvent(Button::KeyAndMouse key);
	std::any GetData() override;

private:

	Button::KeyAndMouse m_key;
};