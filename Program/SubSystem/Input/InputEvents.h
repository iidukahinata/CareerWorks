/**
* @file    InputEvents.h
* @brief   ���͊֌W�̃C�x���g�N���X��`
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "InputHelper.h"

class KeyPressedEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(KeyPressedEvent)
public:

	KeyPressedEvent(Button::KeyAndMouse key);
	std::any GetData() override;

private:

	Button::KeyAndMouse m_key;
};

class KeyReleasedEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(KeyReleasedEvent)
public:

	KeyReleasedEvent(Button::KeyAndMouse key);
	std::any GetData() override;

private:

	Button::KeyAndMouse m_key;
};