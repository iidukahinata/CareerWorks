/**
 * @file	DirectInput.h
 * @brief
 *
 * @date	2022/09/06 2022年度初版
 */
#pragma once


#include <dinput.h>
#include <wrl/client.h>
#include "../Input.h"

class DirectInput : public Input
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(DirectInput)
public:

	bool Initialize() override;
	void Shutdown() override;

	/** 入力イベントチェックを行う。*/
	void Update() noexcept;

private:

	/** 現在のキーボートの入力状態を取得。*/
	bool GetKeyBuffer() noexcept;

	/** 現在のマウスの入力状態を取得。*/
	bool GetMouseBuffer() noexcept;

	/** DirectInput の入力情報を Engine 側で識別できる形に変換をかける。*/
	void ConvertMyInputData() noexcept;

private:

	// * Input Update 登録用
	Job m_job;

	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8>	m_directKeyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8>	m_directMouse;

	// * キーボードバッファ
	Array<char, 256> m_keybuffer;

	// * マウスの状態
	DIMOUSESTATE2 m_mouseState;
};