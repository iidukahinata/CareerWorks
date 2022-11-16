/**
* @file    InputModule.cpp
* @brief
*
* @date	   2022/11/15 2022îNìxèâî≈
*/


#include "ModuleHelper.h"
#include "Subsystem/Window/Window.h"
#include "Subsystem/Input/InputHelper.h"

void ShowMouse(bool isShow)
{
	ShowCursor(isShow);
}

const Math::Vector2& GetMousePosition()
{
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(Window::Get().GetHandle(), &mousePos);
	return Math::Vector2(mousePos.x, mousePos.y);
}

void SetUpInputModule()
{
	using namespace Button;

	PY_ENUM(KeyAndMouse)
		PY_VALUE(Lbutton) PY_VALUE(Rbutton) PY_VALUE(Mbutton)
		PY_VALUE(Back) PY_VALUE(Tab) PY_VALUE(Return) PY_VALUE(Pause) PY_VALUE(Esc) PY_VALUE(Space)
		PY_VALUE(Left) PY_VALUE(Up) PY_VALUE(Right) PY_VALUE(Down)
		PY_VALUE(Select) PY_VALUE(Delete) PY_VALUE(Help)
		PY_VALUE(A) PY_VALUE(B) PY_VALUE(C) PY_VALUE(D) PY_VALUE(E) PY_VALUE(F) PY_VALUE(G)
		PY_VALUE(H) PY_VALUE(I) PY_VALUE(J) PY_VALUE(K) PY_VALUE(L) PY_VALUE(N) PY_VALUE(M)
		PY_VALUE(O) PY_VALUE(P) PY_VALUE(Q)	PY_VALUE(R) PY_VALUE(S)	PY_VALUE(T)	PY_VALUE(U)
		PY_VALUE(V)	PY_VALUE(W)	PY_VALUE(X) PY_VALUE(Y)	PY_VALUE(Z)
		PY_VALUE(F1) PY_VALUE(F2) PY_VALUE(F3) PY_VALUE(F4) PY_VALUE(F5) PY_VALUE(F6)
		PY_VALUE(F7) PY_VALUE(F8) PY_VALUE(F9) PY_VALUE(F10) PY_VALUE(F11) PY_VALUE(F12)
		PY_VALUE(Lshift) PY_VALUE(Rshift) PY_VALUE(Lcontrol) PY_VALUE(Rcontrol) PY_VALUE(Lmenu)
		PY_VALUE(Home) PY_VALUE(End);

	PY_DEF(ShowMouse);
	PY_DEF_POLICIE(GetMousePosition, PY_COPY_CONST_REF);
}