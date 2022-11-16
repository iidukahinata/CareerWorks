/**
* @file		InputHelper.h
* @brief
*
* @date		2022/08/02 2022îNìxèâî≈
*/
#pragma once


namespace Button
{
	enum KeyAndMouse
	{
		// Mouse
		Lbutton,
		Rbutton,
		Mbutton,

		// Keyboard
		Back, Tab, Return, Pause, Esc, Space,
		Left, Up, Right, Down,
		Select, Delete, Help,

		A, B, C, D, E, F, G, H, I, J, K, L, N,
		M, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		Lwin, Rwin, Apps, Sleep = 0x5F,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Lshift, Rshift, Lcontrol, Rcontrol, Lmenu, Rmenu, Home, End,

		Max,
	};
}