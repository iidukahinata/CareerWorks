/**
* @file    Config.h
* @brief
*
* @date	   2022/09/07 2022�N�x����
*/
#pragma once


/**
* @note �T�u�V�X�e�������[�U�[�w��ŕύX����֐������\��
*/
class Config
{
public:

	/** �e�V�X�e���N���X�̐������s���B*/
	static void RegisterSubsystemsToContainer() noexcept;

private:

	static void SetUpDefaultSettings() noexcept;
};