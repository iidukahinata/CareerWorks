/**
 * @file	Context.h
 * @brief	Subsystem �R���e�i�N���X
 *
 * @date	2022/11/27 2022�N�x����
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

	/** �o�^���� T �͎擾�� �w�� Key �N���X�Ƃ��Ď擾���邽�ߌp���֌W�ɂȂ���΂Ȃ�Ȃ��B*/
	template<class Key, class T = Key>
	void RegisterSubsystem();

	template<class Key>
	void UnRegisterSubsystem();

	/** �o�^���Ή������� Key ���g�p���� Subsytem �̎擾�B*/
	template<class Key>
	Key* GetSubsystem();

private:

	/** �L�[�̏d�����������Ƃ��ēo�^�B*/
	void RegisterSubsystem(uint32_t hash, ISubsystem* subsystem) noexcept;
	void UnRegisterSubsystem(uint32_t hash) noexcept;
	ISubsystem* GetSubsystemByHash(uint32_t hash) noexcept;

private:

	// * Type -> <�n�b�V���A�V�X�e���I�u�W�F�N�g>
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

// * �I�u�W�F�N�g
extern Context* g_context;