/**
* @file    InputEvents.cpp
* @brief   ���͊֌W�̃C�x���g�N���X��`
*
* @date	   2022/09/06 2022�N�x����
*/


#include "InputEvents.h"

KeyPressedEvent::KeyPressedEvent(Button::KeyAndMouse key) : m_key(key)
{

}

std::any KeyPressedEvent::GetData()
{
	return m_key;
}

KeyReleasedEvent::KeyReleasedEvent(Button::KeyAndMouse key) : m_key(key)
{

}


std::any KeyReleasedEvent::GetData()
{
	return m_key;
}