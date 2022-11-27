/**
 * @file	Context.h
 * @brief	Subsystem コンテナクラス
 *
 * @date	2022/11/27 2022年度初版
 */
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class Context
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(Context)
public:

	Context() = default;
	void Release();

public:

	/** 登録した T は取得時 指定 Key クラスとして取得するため継承関係になければならない。*/
	template<class Key, class T = Key>
	void RegisterSubsystem();

	template<class Key>
	void UnRegisterSubsystem();

	/** 登録時対応させた Key を使用して Subsytem の取得。*/
	template<class Key>
	Key* GetSubsystem();

private:

	/** キーの重複が無いもとして登録。*/
	void RegisterSubsystem(uint32_t hash, ISubsystem* subsystem) noexcept;
	void UnRegisterSubsystem(uint32_t hash) noexcept;
	ISubsystem* GetSubsystemByHash(uint32_t hash) noexcept;

private:

	// * Type -> <ハッシュ、システムオブジェクト>
	Map<uint32_t, UniquePtr<ISubsystem>> m_subsystems;
};

template<class Key, class T /*= Key */>
FORCEINLINE void Context::RegisterSubsystem()
{
	RegisterSubsystem(Key::TypeData.Hash, new T());
}

template<class Key>
FORCEINLINE void Context::UnRegisterSubsystem()
{
	UnRegisterSubsystem(Key::TypeData.Hash);
}

template<class Key>
FORCEINLINE Key* Context::GetSubsystem()
{
	return dynamic_cast<Key*>(GetSubsystemByHash(Key::TypeData.Hash));
}

// * オブジェクト
extern Context* g_context;