/**
* @file    Config.h
* @brief
*
* @date	   2022/11/09 2022�N�x����
*/
#pragma once


enum class RendererType
{
	Forward,

	Deferred,
};

enum class InputType
{
	Direct,

	None,
};

enum class AudioType
{
	FMOD,

	None,
};

enum class PhysicsType
{
	PhysX,

	None,
};

/**
* @note �T�u�V�X�e�������[�U�[�w��ŕύX����֐������\��
*/
class Config
{
public:

	/** Engine �Ŏg�p�����e�t�@�C���̐������s���B*/
	static void GenerateUseFile() noexcept;

	/** �e�V�X�e���N���X�̐������s���B*/
	static void RegisterSubsystemsToContainer() noexcept;

	/** �w�� RendererSystem �ɐ؂�ւ� */
	static void RegisterRendererSystem(RendererType type, bool saveSettings = true) noexcept;

	/** �w�� InputSystem �ɐ؂�ւ� */
	static void RegisterInputSystem(InputType type, bool saveSettings = true) noexcept;

	/** �w�� AudioSystem �ɐ؂�ւ� */
	static void RegisterAudioSystem(AudioType type, bool saveSettings = true) noexcept;

	/** �w�� PhysicsSystem �ɐ؂�ւ� */
	static void RegisterPhysicsSystem(PhysicsType type, bool saveSettings = true) noexcept;

	static RendererType GetRendererType() noexcept { return m_rendererSystem; }
	static InputType	GetInputType()	  noexcept { return m_inputSystem; }
	static AudioType	GetAudioType()	  noexcept { return m_audioSystem; }
	static PhysicsType	GetPhysicsType()  noexcept { return m_physicsSystem; }

private:

	/** ���ݐݒ�� Subsystem �̏��������s���B*/
	static void SetUpSubsystem() noexcept;

	/** ���ݐݒ�� ����N�����悤�� File �ɕۑ����� */
	static void SaveCurrentConfig() noexcept;

private:

	static RendererType m_rendererSystem;

	static InputType m_inputSystem;

	static AudioType m_audioSystem;

	static PhysicsType m_physicsSystem;
};