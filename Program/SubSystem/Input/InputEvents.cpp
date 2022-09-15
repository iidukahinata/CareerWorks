/**
* @file    InputEvents.cpp
* @brief   入力関係のイベントクラス定義
*
* @date	   2022/09/06 2022年度初版
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