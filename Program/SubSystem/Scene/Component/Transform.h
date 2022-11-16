/**
* @file    Transform.h
* @brief
*
* @date	   2022/10/03 2022îNìxèâî≈
*/
#pragma once


class GameObject;

class Transform
{
	COMPLETED_DEVELOPMENT()
	CLASS_DATA(Transform)
public:

	Transform() = default;
	Transform(GameObject* gameObject);
	~Transform();

	void Serialized(FileStream* file) const noexcept;
	void Deserialized(FileStream* file) noexcept;

public:

	void SetPosition(const Math::Vector3& position) noexcept;
	const Math::Vector3& GetPosition() const noexcept;
	const Math::Vector3& GetWoldPosition() const;

	void SetRotation(const Math::Vector3& rotation) noexcept;
	const Math::Vector3& GetRotation() const noexcept;

	void SetScale(const Math::Vector3& scale) noexcept;
	const Math::Vector3& GetScale() const noexcept;

	Math::Matrix GetLocalMatrix() const;
	Math::Matrix GetWorldMatrix() const;

	const Math::Vector3& GetRight() const noexcept;
	const Math::Vector3& GetUp() const noexcept;
	const Math::Vector3& GetForward() const noexcept;

	void LockAt(const Math::Vector3& target, const Math::Vector3& up = Math::Vector3::Right);
	void LockTo(const Math::Vector3& direction, const Math::Vector3& up = Math::Vector3::Right);

public:

	bool HasParent() const noexcept;
	void SetParent(Transform* parent) noexcept;
	void AddChild(Transform* child) noexcept;
	void RemoveChild(Transform* child) noexcept;

	uint32_t GetChildCount() const noexcept;
	const Vector<Transform*>& GetChildren() const noexcept;
	Transform* GetParent() const noexcept;
	Transform* GetRoot() const noexcept;

	GameObject* GetOwner() const noexcept;

private:

	GameObject* m_owner = nullptr;

	Transform* m_parent = nullptr;
	Vector<Transform*> m_children;

	Math::Vector3 m_localPosition;
	Math::Vector3 m_localRotation;
	Math::Vector3 m_localScale;

	Math::Vector3 m_up;
	Math::Vector3 m_forward;
	Math::Vector3 m_right;
};