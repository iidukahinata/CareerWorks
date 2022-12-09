/**
* @file    Config.h
* @brief
*
* @date	   2022/11/09 2022年度初版
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
* @note サブシステムをユーザー指定で変更する関数実装予定
*/
class Config
{
public:

	/** Engine で使用される各ファイルの生成を行う。*/
	static void GenerateUseFile() noexcept;

	/** 各システムクラスの生成を行う。*/
	static void RegisterSubsystemsToContainer() noexcept;

	/** 指定 RendererSystem に切り替え */
	static void RegisterRendererSystem(RendererType type, bool saveSettings = true) noexcept;

	/** 指定 InputSystem に切り替え */
	static void RegisterInputSystem(InputType type, bool saveSettings = true) noexcept;

	/** 指定 AudioSystem に切り替え */
	static void RegisterAudioSystem(AudioType type, bool saveSettings = true) noexcept;

	/** 指定 PhysicsSystem に切り替え */
	static void RegisterPhysicsSystem(PhysicsType type, bool saveSettings = true) noexcept;

	static RendererType GetRendererType() noexcept { return m_rendererSystem; }
	static InputType	GetInputType()	  noexcept { return m_inputSystem; }
	static AudioType	GetAudioType()	  noexcept { return m_audioSystem; }
	static PhysicsType	GetPhysicsType()  noexcept { return m_physicsSystem; }

private:

	/** 現在設定で Subsystem の初期化を行う。*/
	static void SetUpSubsystem() noexcept;

	/** 現在設定を 次回起動時ように File に保存する */
	static void SaveCurrentConfig() noexcept;

private:

	static RendererType m_rendererSystem;

	static InputType m_inputSystem;

	static AudioType m_audioSystem;

	static PhysicsType m_physicsSystem;
};