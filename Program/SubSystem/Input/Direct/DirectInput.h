/**
 * @file	DirectInput.h
 * @brief
 *
 * @date	2022/09/06 2022�N�x����
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

	/** ���̓C�x���g�`�F�b�N���s���B*/
	void Update() noexcept;

private:

	/** ���݂̃L�[�{�[�g�̓��͏�Ԃ��擾�B*/
	bool GetKeyBuffer() noexcept;

	/** ���݂̃}�E�X�̓��͏�Ԃ��擾�B*/
	bool GetMouseBuffer() noexcept;

	/** DirectInput �̓��͏��� Engine ���Ŏ��ʂł���`�ɕϊ���������B*/
	void ConvertMyInputData() noexcept;

private:

	// * Input Update �o�^�p
	Job m_job;

	Microsoft::WRL::ComPtr<IDirectInput8> m_directInput;
	Microsoft::WRL::ComPtr<IDirectInputDevice8>	m_directKeyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8>	m_directMouse;

	// * �L�[�{�[�h�o�b�t�@
	Array<char, 256> m_keybuffer;

	// * �}�E�X�̏��
	DIMOUSESTATE2 m_mouseState;
};