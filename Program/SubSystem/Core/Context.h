/**
 * @file	Context.h
 * @brief	Subsystem コンテナクラス
 *
 * @date	2022/09/06 2022年度初版
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

	/** 登録した T は取得時 指定 Key クラスとして取得するため継承関係になければならない。*/
	template<class Key, class T>
	void RegisterSubsystem(UniquePtr<T> subsystem);

	/** 登録時対応させた Key を使用して Subsytem の取得。*/
	template<class Key>
	Key* GetSubsystem();

private:

	/** キーの重複が無いもとして登録。*/
	void RegisterSubsystem(uint32_t hash, ISubsystem* subsystem) noexcept;
	ISubsystem* GetSubsystemByHash(uint32_t hash) noexcept;

private:

	// * Type -> <ハッシュ、システムオブジェクト>
	Map<uint32_t, UniquePtr<ISubsystem>> m_subsystems;
};

template<class Key, class T>
FORCEINLINE void Context::RegisterSubsystem(UniquePtr<T> subsystem)
{
	RegisterSubsystem(Key::TypeData.Hash, subsystem.release());
}

template<class Key>
FORCEINLINE Key* Context::GetSubsystem()
{
	return dynamic_cast<Key*>(GetSubsystemByHash(Key::TypeData.Hash));
}