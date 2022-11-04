/**
* @file    IComponent.h
* @brief
*
* @date	   2022/10/25 2022�N�x����
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
	virtual void Deserialized(FileStream* file);

public:

	/** �V�[�����[�h���̉\�������邽�� GameObject / Component �̌����͍s��Ȃ������ǂ��B*/
	virtual void OnInitialize();

	/** world �o�^���ɌĂяo�����B*/
	virtual void OnRegister();

	/** world ����o�^�������ɌĂяo�����B*/
	virtual void OnUnRegister();

	/** �Q�[���I�u�W�F�N�g�̍Đ��� or ���ɍĐ�����Ă��鎞�̓R���|�[�l���g�������ɌĂяo�����B*/
	virtual void OnStart();

	/** �R���|�[�l���g�̍Đ��I�����ɌĂяo�����B*/
	virtual void OnStop();

	/** GameObject �����������^�C�~���O�ŌĂяo�����B*/
	virtual void OnRemove();

	/** �h����Ŏg�p����ꍇ�́ATickFunction ���V�X�e�����ɓo�^����K�v������܂��B*/
	virtual void Tick(double deltaTime);

	/** 
	* �ʃX���b�h�ő��삳��Ă��鎞�� Remove �\���������Ă����S�ɋ@�\���邽�߂̃t���O�֐�
	* Remove ���� true �̏ꍇ�͂��̏�ŕ�������A false �̏ꍇ�� AutoDestroyManager �֊Ǘ�����n���B
	*/
	virtual bool Erasable();

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

	bool IsRegistered() const noexcept;
	bool IsBeginPlay() const noexcept;

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

	// * �����Q�[���I�u�W�F�N�g�N���X��ێ�
	GameObject* m_owner = nullptr;

	// * �R���|�[�l���g�̗L������ێ�
	bool m_active = true;

	// * world �ɓo�^����Ă��邩��ێ�
	bool m_registered = false;

	// * �R���|�[�l���g���Đ������ǂ�����ێ�
	bool m_beginPlay = false;
};

template<class T>
FORCEINLINE bool IComponent::IsSameClass() const noexcept
{
	return TypeInfo() == typeid(T);
}