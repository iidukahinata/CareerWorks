/**
* @file    GameObject.h
* @brief
*
* @date	   2022/10/25 2022�N�x����
*/
#pragma once


#include "Component/IComponent.h"
#include "Component/Transform.h"

class GameObject
{
	COMPLETED_DEVELOPMENT()
	CLASS_DATA(GameObject)
	COPY_PROHIBITED(GameObject)

	friend struct GameObjectFactory;
public:

	GameObject();

	void Serialized(FileStream* file) const noexcept;
	void Deserialization(FileStream* file) noexcept;

public:

	/** component �� world �o�^���s���B*/
	void RegisterAllComponents() noexcept;

	/** component �� world �o�^�������s���B*/
	void UnRegisterAllComponents() noexcept;

	/** �Q�[���I�u�W�F�N�g�̍Đ����ɌĂяo�����B*/
	void BeginPlay() noexcept;

	/** �Q�[���I�u�W�F�N�g�̏I�����ɌĂяo�����B*/
	void EndPlay() noexcept;

public:

	/** �w�薼�R���|�[�l���g�̒ǉ��B�w�薼�����݂��Ȃ����̏ꍇ�Anull ��Ԃ��B*/
	IComponent* AddComponent(StringView name) noexcept;
	void AddComponent(IComponent* component) noexcept;

	/** �ێ�����R���|�[�l���g���������邽�߁A���̃A�h���X�������Ƃ���B*/
	void RemoveComponent(IComponent* component) noexcept;

	/** �^������R���|�[�l���g�������B*/
	template<class T>
	T* GetComponent();
	const Map<uint32_t, UniquePtr<IComponent>>& GetAllComponent() const noexcept;

	/** �w�薼�R���|�[�l���g��ێ�����ꍇ�A���̃A�h���X��Ԃ��B*/
	IComponent* FindComponent(StringView name) const noexcept;

	void ClearComponets() noexcept;

public:

	void SetActive(bool active) noexcept;
	bool GetActive() const noexcept;

	/** �eGameObject���ʗp�Ɏg�p�B*/
	void SetName(StringView name) noexcept;
	const String& GetName() const noexcept;

	/** World�N���X�ňȊO�Ŏg�p�����ƁA�ꍇ�ɂ���Ă�Scene�I�����܂Ń�������������ꂸ�c�葱���܂��B*/
	void SetID(uint32_t id) noexcept;
	uint32_t GetID() const noexcept;

	/** �A�N�Z�X */
	Transform& GetTransform() noexcept;
	Scene* GetOwner() const noexcept;
	World* GetWorld() const noexcept;
	Context* GetContext() const noexcept;

private:

	// * �����V�[���N���X��ێ�
	Scene* m_owner = nullptr;

	// * �������[���h�N���X��ێ�
	World* m_world = nullptr;

	// * World�N���X�ł�ID�l
	uint32_t m_id = 0;

	// * World�N���X�ŒT�������Ɏg�p
	String m_path = "GameObject";

	// * �R���|�[�l���g�̗L������ێ�
	bool m_active = true;

	// * world �ɓo�^����Ă��邩��ێ�
	bool m_registered = false;

	// * �Q�[���I�u�W�F�N�g���Đ����ł��邩��ێ�
	bool m_isPlaying = false;

	Transform m_transform;

	// * -> �n�b�V���l : �e�R���|�[�l���g�I�u�W�F�N�g
	Map<uint32_t, UniquePtr<IComponent>> m_components;
};

template<class T>
FORCEINLINE T* GameObject::GetComponent()
{
	return dynamic_cast<T*>(FindComponent(T::TypeData.Name));
}