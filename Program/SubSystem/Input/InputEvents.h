/**
* @file    InputEvents.h
* @brief   ���͊֌W�̃C�x���g�N���X��`
*
* @date	   2022/11/28 2022�N�x����
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