/**
* @file    Matrix.cpp
* @brief
*
* @date	   2022/07/09 2022”N“x‰”Å
*/


#include "Matrix.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace Math
{
	const Matrix Matrix::Identity(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	Matrix::Matrix(const Vector4& InX, const Vector4& InY, const Vector4& InZ, const Vector4& InW) noexcept
	{
		m[0][0] = InX.x; m[0][1] = InX.y;  m[0][2] = InX.z;  m[0][3] = InX.w;
		m[1][0] = InY.x; m[1][1] = InY.y;  m[1][2] = InY.z;  m[1][3] = InY.w;
		m[2][0] = InZ.x; m[2][1] = InZ.y;  m[2][2] = InZ.z;  m[2][3] = InZ.w;
		m[3][0] = InW.x; m[3][1] = InW.y;  m[3][2] = InW.z;  m[3][3] = InW.w;
	}

	Matrix::Matrix(const Vector3& position, const Quaternion& rotation, const Vector3& scale) noexcept
	{
		Vector4 v(rotation.x, rotation.y, rotation.z, rotation.w);

		auto world = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale)) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&v)) *
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));

		DirectX::XMStoreFloat4x4(this, world);
	}

	Matrix::Matrix(const Vector3& position, const Vector3& angles, const Vector3& scale) noexcept
	{
		auto world = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale)) *
			DirectX::XMMatrixRotationZ(angles.z) *
			DirectX::XMMatrixRotationX(angles.x) *
			DirectX::XMMatrixRotationY(angles.y) *
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));

		DirectX::XMStoreFloat4x4(this, world);
	}

	Matrix Matrix::CreateTranslation(const Vector3& position) noexcept
	{
		Matrix translation;
		DirectX::XMStoreFloat4x4(&translation, DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position)));
		return translation;
	}

	Matrix Matrix::CreateRotation(const Quaternion& rotation) noexcept
	{
		Vector4 v(rotation.x, rotation.y, rotation.z, rotation.w);

		Matrix result;
		DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&v)));
		return result;
	}

	Matrix Matrix::CreateRotation(const Vector3& angles) noexcept
	{
		Matrix rotation;
		DirectX::XMStoreFloat4x4(
			&rotation,
			DirectX::XMMatrixRotationZ(angles.z) *
			DirectX::XMMatrixRotationX(angles.x) *
			DirectX::XMMatrixRotationY(angles.y));

		return rotation;
	}

	Matrix Matrix::CreateScaling(const Vector3& scale) noexcept
	{
		Matrix scaling;
		DirectX::XMStoreFloat4x4(&scaling, DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale)));
		return scaling;
	}

	Matrix Matrix::CreatePerspectiveFovLH(float fov, float aspect, float nearZ, float farZ) noexcept
	{
		Matrix projection;
		DirectX::XMStoreFloat4x4(&projection, DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ));
		return projection;
	}

	Matrix Matrix::CreateOrthographicLH(float width, float height, float nearZ, float farZ) noexcept
	{
		Matrix orthographic;
		DirectX::XMStoreFloat4x4(&orthographic, DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ));
		return orthographic;
	}

	Vector3 Matrix::TransformPosition(const Vector3& position) const noexcept
	{
		Vector3 result;
		DirectX::XMStoreFloat3(&result, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&position), ToMatrixXM()));
		return result;
	}

	Vector3 Matrix::GetTranslation() const noexcept
	{
		return Vector3(m[3][0], m[3][1], m[3][2]);
	}

	Quaternion Matrix::GetQuaternion() const noexcept
	{
		return Quaternion::FromEuler(GetEulerAngles());
	}

	Vector3 Matrix::GetEulerAngles() const noexcept
	{
		if (_32 >= 0.9999f)
		{
			return Vector3(-PI / 2.f, 0.f, -atan2f(_13, _11)) * (180.0f / PI);
		}
		if (_32 <= -0.9999f)
		{
			return Vector3(PI / 2.f, 0.f, atan2f(_13, _11)) * (180.0f / PI);
		}
		return Vector3(-asin(_32), -atan2f(-_31, _33), -atan2f(-_12, _22)) * (180.0f / PI);
	}

	Vector3 Matrix::GetScale() const noexcept
	{
		Vector3 row0(m[0][0], m[0][1], m[0][2]);
		Vector3 row1(m[1][0], m[1][1], m[1][2]);
		Vector3 row2(m[2][0], m[2][1], m[2][2]);

		return Vector3(row0.GetLenght(), row1.GetLenght(), row2.GetLenght());
	}
}