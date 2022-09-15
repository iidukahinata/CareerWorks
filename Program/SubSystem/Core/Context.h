/**
 * @file	Context.h
 * @brief	Subsystem �R���e�i�N���X
 *
 * @date	2022/09/06 2022�N�x����
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

	/** �o�^���� T �͎擾�� �w�� Key �N���X�Ƃ��Ď擾���邽�ߌp���֌W�ɂȂ���΂Ȃ�Ȃ��B*/
	template<class Key, class T>
	void RegisterSubsystem(UniquePtr<T> subsystem);

	/** �o�^���Ή������� Key ���g�p���� Subsytem �̎擾�B*/
	template<class Key>
	Key* GetSubsystem();

private:

	/** �L�[�̏d�����������Ƃ��ēo�^�B*/
	void RegisterSubsystem(uint32_t hash, ISubsystem* subsystem) noexcept;
	ISubsystem* GetSubsystemByHash(uint32_t hash) noexcept;

private:

	// * Type -> <�n�b�V���A�V�X�e���I�u�W�F�N�g>
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