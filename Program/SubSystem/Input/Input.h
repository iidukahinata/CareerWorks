/**
 * @file	Input.h
 * @brief   入力制御クラス
 *
 * @date	2022/09/06 2022年度初版
 */
#pragma once


#include "InputHelper.h"
#include "SubSystem/Core/ISubsystem.h"

/**
* Input抽象クラス
* このクラスではインターフェースのみを実装。
* 派生クラスで初期化等の関数を実装する。
*/
class Input : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Input)
public:

	/** マウス位置をスクリーン座標で返します */
	virtual const Math::Vector2& GetMousePosition() const noexcept;

	/** マウス表示を管理します */
	virtual void ShowMouse(bool show) const noexcept;

protected:

	/** 各派生 Input クラスではこの関数を使用し PressEvent を知らせます。*/
	void NotifyPressKey(Button::KeyAndMouse id) const noexcept;

	/** 各派生 Input クラスではこの関数を使用し ReleaseEvent を知らせます。*/
	void NotifyReleaseKey(Button::KeyAndMouse id) const noexcept;

protected:

	// 任意キーが押されているかを保持
	Array<bool, Button::KeyAndMouse::Max> m_previousKeyState = { false };
};