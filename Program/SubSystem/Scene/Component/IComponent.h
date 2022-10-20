/**
* @file    IComponent.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


#include "Transform.h"
#include "../TickFunction.h"

class Context;
class World;
class Scene;
class GameObject;

using ComponentType = ClassTypeData;

class IComponent
{
	COMPLETED_DEVELOPMENT()
	SUPER_CLASS(IComponent)
	COPY_PROHIBITED(IComponent)

	friend struct ComponentFactory;
public:

	IComponent();

	virtual void Serialized(FileStream* file) const;
	virtual void Deserialization(FileStream* file);

public:

	/** �V�[�����[�h���̉\�������邽�� GameObject / Component �̌����͍s��Ȃ������ǂ��B*/
	virtual void OnInitialize() {}

	/** ���� Scene �� AddToWorld �K�p���ɌĂяo�����B*/
	virtual void OnStart() {}

	/** ���� Scene �� RemoveFromWorld �K�p���ɌĂяo�����B*/
	virtual void OnStop() {}

	/** GameObject �����������^�C�~���O�ŌĂяo�����B*/
	virtual void OnRemove() { OnStop(); }

	/** �h����Ŏg�p����ꍇ�́ATickFunction ���V�X�e�����ɓo�^����K�v������܂��B*/
	virtual void Tick(double deltaTime) {}

public:

	/** true �̏ꍇ�A�X�V�֌W�ɂ��e������B*/
	virtual void SetActive(bool active);
	bool GetActive() const noexcept;

	/** true �̏ꍇ�A�X�V�������L���ɂȂ�܂��B*/
	void SetTickEnable(bool enable) noexcept;

	/** �w��D��x�����ɍX�V������ύX���܂��B�ő�l : 0 */
	void SetTickPriority(uint32_t priority) noexcept;

	/** �X�V�����̑O�������ݒ肵�܂��B*/
	void AddTickPrerequisite(TickFunction* prerequisite) noexcept;

public:

	/** �e���v���[�g�^�� this �Ɠ����^�̎� true ��Ԃ��܂��B*/
	template<class T>
	bool IsSameClass() const noexcept;

	/** �A�N�Z�X */
	Transform& GetTransform() const noexcept;
	GameObject* GetOwner() const noexcept;
	Scene* GetScene() const noexcept;
	World* GetWorld() const noexcept;
	Context* GetContext() const noexcept;

protected:

	// * �R���|�[�l���g�̍X�V�������Ǘ�
	TickComponentFunction m_tickFunction;

private:

	bool m_active;

	GameObject* m_owner = nullptr;
};

template<class T>
FORCEINLINE bool IComponent::IsSameClass() const noexcept
{
	return TypeInfo() == typeid(T);
}