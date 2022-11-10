/**
* @file		JobHelper.h
* @brief
*
* @date		2022/09/07 2022�N�x����
*/
#pragma once


/**
* Job ���s�^�C�~���O����p enum
*/
enum FunctionType
{
	// �t���[���J�n���Ɏ��s�B��Ɋe Thread �̗����グ���s���B
	PreUpdate,

	// �����v�Z�O�Ɏ��s�BWorld �X�V���s���B
	PrePhysics,

	// �����v�Z�̊J�n���Ɏ��s�B
	StartPhysics,

	// �����v�Z��Ɏ��s�B
	EndPhysics,

	// �S�Ă̍X�V���I���������Ɏ��s�B
	PostUpdate,

	// �`��J�n���Ɏ��s�B
	Render,

	// �S�Ă̕`�揈�����I���������Ɏ��s���BPreset �Ȃǂ��s���B
	PostRender,

	None,
};