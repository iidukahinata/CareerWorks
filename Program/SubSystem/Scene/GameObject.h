/**
* @file    GameObject.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "Component/IComponent.h"
#include "Component/Transform.h"

class GameObject
{
	COMPLETED_DEVELOPMENT()
	CLASS_DATA(GameObject)

	friend struct GameObjectFactory;
public:

	void Serialized(FileStream* file) const;
	void Deserialization(FileStream* file);

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
	IComponent* FindComponent(StringView name) noexcept;

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

	// * �����V�[���N���X��ێ��B
	Scene* m_owner;

	// * �������[���h�N���X��ێ��B
	World* m_world;

	// * World�N���X�ł�ID�l
	uint32_t m_id;

	// * World�N���X�ŒT�������Ɏg�p�����B
	String m_path;

	bool m_active = true;

	Transform m_transform;

	// * -> �n�b�V���l : �e�R���|�[�l���g�I�u�W�F�N�g
	Map<uint32_t, UniquePtr<IComponent>> m_components;
};

template<class T>
FORCEINLINE T* GameObject::GetComponent()
{
	return dynamic_cast<T*>(FindComponent(T::TypeData.Name));
}