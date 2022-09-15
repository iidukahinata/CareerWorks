/**
* @file    Input.cpp
* @brief
*
* @date	2022/09/06 2022îNìxèâî≈
*/


#include "Input.h"
#include "InputEvents.h"
#include "SubSystem/Window/Window.h"

void Input::ShowMouse(bool isShow) const noexcept
{
	ShowCursor(isShow);
}

const Math::Vector2& Input::GetMousePosition() const noexcept
{
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(Window::Get().GetHandle(), &mousePos);
	return Math::Vector2(mousePos.x, mousePos.y);
}

void Input::NotifyPressKey(Button::KeyAndMouse id) const noexcept
{
	NotifyEvent<KeyPressedEvent>(id);
}

void Input::NotifyReleaseKey(Button::KeyAndMouse id) const noexcept
{
	NotifyEvent<KeyReleasedEvent>(id);
}