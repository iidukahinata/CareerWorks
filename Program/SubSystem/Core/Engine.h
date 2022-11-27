/**
* @file    Engine.h
* @brief
*
* @date	   2022/11/27 2022�N�x����
*/
#pragma once


#include <Windows.h>

class Timer;

class Engine
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(Engine)
public:

	Engine() = default;

	/** �A�v���P�[�V�����S�̂̃Z�b�g�A�b�v���s���܂��B*/
	bool Initialize(HINSTANCE hInstance);

	/** �G���W�����[�v���������܂��B*/
	long MainLoop();

	/**
	* ���̊֐��� JobSystem ����������O�ɌĂяo���K�v������܂��B
	* �e�V�X�e�����Z�b�g���� Job ��������̂��� JobSystem �ɃA�N�Z�X���邽�߂ł��B
	*/
	void Shutdown();

private:

	/** ��ɁA�ʏ�̃E�B���h�E�ƃf�o�b�N�E�B���h�E�𐶐����܂��B*/
	bool StartupScreen(HINSTANCE hInstance) const noexcept;

	/** Thread System And Runnable Thread �̗����グ���s���B*/
	bool StartupThread();

	/** �o�^���ꂽ�e�V�X�e���̃Z�b�g�A�b�v���s���B*/
	bool InitializeSubsystems() noexcept;

private:

	void Tick() const noexcept;

private:

	HINSTANCE m_hInstance;

	Timer* m_timer;

	UniquePtr<Context> m_context;
};