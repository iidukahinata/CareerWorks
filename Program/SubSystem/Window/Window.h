/**
* @file    Window.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include <Windows.h>

class Window
{
	COMPLETED_DEVELOPMENT()
	Window() = default;
	COPY_PROHIBITED(Window)
public:

	static Window& Get() noexcept
	{
		static Window instance;
		return instance;
	}

public:

	/**
	* �ݒ肳�ꂽ�l�� Window ���쐬
	* 
	* @param hInstance [����] ���s���̃C���X�^���X�n���h�����w�肷��K�v������܂��B
	* @param width [����] �O�ȉ��̒l���w�肵�Ȃ��ł��������B
	* @param height [����] �O�ȉ��̒l���w�肵�Ȃ��ł��������B
	* @param title [����] �������� Window �̃^�C�g���o�[�ɏo�͂��镶����Bnull�͍l�����Ă��Ȃ��B
	* @param fullScreen [����] true���w�肳�ꂽ���� hInstance �ȊO�̈����l�̈Ӗ��͂Ȃ��Ȃ�܂��B
	*/
	bool CreateWindowClass(HINSTANCE hInstance, int width, int height, StringView title, bool fullScreen) noexcept;

	/**
	* �A�v���P�[�V�����Ɋւ��郁�b�Z�[�W���擾���A
	* window message ���� window ��Ԃ𒲂ׂ�
	* 
	* @return false �̏ꍇ�� window �̏I�����}
	*/
	bool Tick();

public:

	/** �Ō�ɍX�V���ꂽ���� window ��Ԃ̏ڍ׏���Ԃ��܂��B*/
	long GetMessage() const noexcept;

	/** �������Ɏw�肳�ꂽ���s���C���X�^���X�n���h����Ԃ��B*/
	HINSTANCE GetHInstance() const noexcept;

	/** Create �Ő������ꂽ Window �n���h���I�u�W�F�N�g��Ԃ��B*/
	HWND GetHandle() const noexcept;

	/** window size */
	int	 GetWindowWidth() const noexcept;
	int	 GetWindowHeight() const noexcept;
	bool IsFullscreen() const noexcept;

private:

	// * ���s���C���X�^���X�n���h��
	HINSTANCE m_hInstance;

	// * �������� window ���ʎq
	HWND m_hWnd;

	//*  window ��Ԃ��O������擾���邽�߂̕ϐ�
	MSG m_message;

	// * window size
	int m_width;
	int m_height;
	bool m_fullscreen;
};