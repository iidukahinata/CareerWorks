/**
* @file    ComponentFactory.h
* @brief
*
* @date	   2022/11/27 2022�N�x����
*/
#pragma once


#include "../Component/IComponent.h"

/**
* �R���|�[�l���g�̐������V�X�e�����番�������邽�߂ɍ쐬�B
* �V�����R���|�[�l���g���쐬�����ꍇ�A���L�֐����ɒǉ����Ă����B
*/
struct ComponentFactory
{
public:

	/** �w�薼�̃R���|�[�l���g�̐������s���B��������Ȃ��ꍇ�� null �I�u�W�F�N�g��Ԃ��B*/
	static UniquePtr<IComponent> Create(GameObject* gameObject, StringView name) noexcept;

	/** �Ăяo�����ɓo�^����Ă���R���|�[�l���g�^�C�v�iKey�j��Ԃ��܂��B*/
	static Vector<StringView> GetAllComponentType();

public:

	/** system �N������ system �ɑΉ�����e�R���|�[�l���g�̓o�^�� system ����s���B*/
	template<class Key, class T = Key>
	static void Register();

	/** system �I�����ɑΉ�����R���|�[�l���g�̓o�^�������s���悤�ɂ���B*/
	template<class Key>
	static void UnRegister();

private:

	// * �g�p�R���|�[�l���g�̓V�X�e���Ɉˑ����邽�ߐ����֐���o�^����`�ɂ���B
	static Unordered_Map<String, std::function<UniquePtr<IComponent>()>> m_componentFactorys;
};

template<class Key, class T /* = Key */>
FORCEINLINE void ComponentFactory::Register()
{
	auto name = Key::TypeData.Name;
	if (!m_componentFactorys.contains(name.data()))
	{
		m_componentFactorys[name.data()] = []() { return std::make_unique<T>(); };
	}
}

template<class Key>
FORCEINLINE void ComponentFactory::UnRegister()
{
	auto name = Key::TypeData.Name;
	if (m_componentFactorys.contains(name))
	{
		m_componentFactorys.erase(name);
	}
}