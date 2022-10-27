/**
 * @file	Input.h
 * @brief   ���͐���N���X
 *
 * @date	2022/09/06 2022�N�x����
 */
#pragma once


#include "InputHelper.h"
#include "SubSystem/Core/ISubsystem.h"

/**
* Input���ۃN���X
* ���̃N���X�ł̓C���^�[�t�F�[�X�݂̂������B
* �h���N���X�ŏ��������̊֐�����������B
*/
class Input : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Input)
public:

	/** �}�E�X�ʒu���X�N���[�����W�ŕԂ��܂� */
	virtual const Math::Vector2& GetMousePosition() const noexcept;

	/** �}�E�X�\�����Ǘ����܂� */
	virtual void ShowMouse(bool show) const noexcept;

protected:

	/** �e�h�� Input �N���X�ł͂��̊֐����g�p�� PressEvent ��m�点�܂��B*/
	void NotifyPressKey(Button::KeyAndMouse id) const noexcept;

	/** �e�h�� Input �N���X�ł͂��̊֐����g�p�� ReleaseEvent ��m�点�܂��B*/
	void NotifyReleaseKey(Button::KeyAndMouse id) const noexcept;

protected:

	// �C�ӃL�[��������Ă��邩��ێ�
	Array<bool, Button::KeyAndMouse::Max> m_previousKeyState = { false };
};