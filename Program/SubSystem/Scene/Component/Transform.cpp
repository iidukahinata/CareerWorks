/**
* @file    Transform.cpp
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/


#include "Transform.h"

Transform::Transform(GameObject* gameObject) :
	m_owner(gameObject),
	m_localPosition(0.f, 0.f, 0.f),
	m_localRotation(0.f, 0.f, 0.f),
	m_localScale(1.f, 1.f, 1.f),
	m_right(1.f, 0.f, 0.f),
	m_up(0.f, 1.f, 0.f),
	m_forward(0.f, 0.f, 1.f)
{
	ASSERT(m_owner);
}

Transform::~Transform()
{
	SetParent(nullptr);

	for (auto child : m_children)
	{
		child->SetParent(nullptr);
	}
}

void Transform::Serialized(FileStream* file) const noexcept
{
	file->Write(m_localPosition);
	file->Write(m_localRotation);
	file->Write(m_localScale);
}

void Transform::Deserialized(FileStream* file) noexcept
{
	file->Read(&m_localPosition);
	file->Read(&m_localRotation);
	file->Read(&m_localScale);

	// right up forward ì¬—p
	SetRotation(m_localRotation);
}

void Transform::SetPosition(const Math::Vector3& position) noexcept
{
	m_localPosition = position;
}

const Math::Vector3& Transform::GetPosition() const noexcept
{
	return m_localPosition;
}

const Math::Vector3& Transform::GetWoldPosition() const
{
	return GetWorldMatrix().GetTranslation();
}

void Transform::SetRotation(const Math::Vector3& rotation) noexcept
{
	m_localRotation = rotation;

	auto rot  = Math::Matrix::CreateRotation(m_localRotation);
	m_right   = Math::Vector3(rot.m[0][0], rot.m[0][1], rot.m[0][2]);
	m_up      = Math::Vector3(rot.m[1][0], rot.m[1][1], rot.m[1][2]);
	m_forward = Math::Vector3(rot.m[2][0], rot.m[2][1], rot.m[2][2]);
}

const Math::Vector3& Transform::GetRotation() const noexcept
{
	return m_localRotation;
}

void Transform::SetScale(const Math::Vector3& scale) noexcept
{
	m_localScale = scale;
}

const Math::Vector3& Transform::GetScale() const noexcept
{
	return m_localScale;
}

void Transform::LockAt(const Math::Vector3& target, const Math::Vector3& up /* = Math::Vector3::Right */)
{
	auto view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&m_localPosition),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&up));

	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, std::move(view));
	Math::Matrix rotMatrix;
	DirectX::XMStoreFloat4x4(&rotMatrix, std::move(invView));
	SetRotation(rotMatrix.GetEulerAngles());
}

void Transform::LockTo(const Math::Vector3& direction, const Math::Vector3& up /* = Math::Vector3::Right */)
{
	auto view = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat3(&m_localPosition),
		DirectX::XMLoadFloat3(&direction),
		DirectX::XMLoadFloat3(&up));

	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, std::move(view));
	Math::Matrix rotMatrix;
	DirectX::XMStoreFloat4x4(&rotMatrix, std::move(invView));
	SetRotation(rotMatrix.GetEulerAngles());
}

Math::Matrix Transform::GetLocalMatrix() const
{
	return Math::Matrix(m_localPosition, m_localRotation, m_localScale);
}

Math::Matrix Transform::GetWorldMatrix() const
{
	auto result = GetLocalMatrix();
	auto parent = m_parent;
	while (parent)
	{
		result *= m_parent->GetLocalMatrix();
		parent = parent->m_parent;
	}
	return result;
}

const Math::Vector3& Transform::GetRight() const noexcept
{
	return m_right;
}

const Math::Vector3& Transform::GetUp() const noexcept
{
	return m_up;
}

const Math::Vector3& Transform::GetForward() const noexcept
{
	return m_forward;
}

bool Transform::HasParent() const noexcept
{
	return !!m_parent;
}

void Transform::SetParent(Transform* parent) noexcept
{
	if (m_parent)
	{
		m_parent->RemoveChild(this);
	}

	if (parent)
	{
		parent->AddChild(this);
	}

	m_parent = parent;
}

void Transform::AddChild(Transform* child) noexcept
{
	m_children.emplace_back(child);
}

void Transform::RemoveChild(Transform* child) noexcept
{
	std::erase(m_children, child);
}

Transform* Transform::GetParent() const noexcept
{
	return m_parent;
}

uint32_t Transform::GetChildCount() const noexcept
{
	return m_children.size();
}

const Vector<Transform*>& Transform::GetChildren() const noexcept
{
	return m_children;
}

Transform* Transform::GetRoot() const noexcept
{
	Transform* root = nullptr;
	Transform* parent = m_parent;
	while (parent)
	{
		root = parent;
		parent = parent->m_parent;
	}
	return root;
}

GameObject* Transform::GetOwner() const noexcept
{
	return m_owner;
}