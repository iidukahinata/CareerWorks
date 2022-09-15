/**
* @file    Matrix.h
* @brief
*
* @date	   2022/07/09 2022年度初版
*/
#pragma once


#include"MathHelper.h"

namespace Math 
{
	struct Matrix : public DirectX::XMFLOAT4X4
	{
		COMPLETED_DEVELOPMENT()
	public:

		static const Matrix Identity;

	public:

		constexpr Matrix() noexcept : DirectX::XMFLOAT4X4(Identity)
		{}

		constexpr Matrix(float m00, float m01, float m02, float m03,
						 float m10, float m11, float m12, float m13,
						 float m20, float m21, float m22, float m23,
						 float m30, float m31, float m32, float m33) noexcept :
			DirectX::XMFLOAT4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33)
		{}

		Matrix(const Vector4& InX, const Vector4& InY, const Vector4& InZ, const Vector4& InW) noexcept;

		Matrix(const Vector3& position, const Vector3& eulerAngles, const Vector3& scale) noexcept;

		Matrix operator+(const Matrix& Other) const noexcept
		{
			Matrix result = *this;
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					result.m[x][y] += Other.m[x][y];
				}
			}
			return result;
		}

		Matrix operator*(const Matrix& Other) const noexcept
		{
			Matrix mul;
			DirectX::XMStoreFloat4x4(&mul, DirectX::XMMatrixMultiply(ToMatrixXM(), Other.ToMatrixXM()));
			return mul;
		}

		constexpr Matrix operator*(float value) const noexcept
		{
			auto result = *this;
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					result.m[x][y] *= value;
				}
			}
			return result;
		}

		constexpr Matrix operator+=(const Matrix& Other) noexcept
		{			
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					m[x][y] += Other.m[x][y];
				}
			}
			return *this;
		}

		Matrix operator*=(const Matrix& Other) noexcept
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixMultiply(ToMatrixXM(), Other.ToMatrixXM()));
			return *this;
		}

		constexpr Matrix operator*=(float value) noexcept
		{
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					m[x][y] *= value;
				}
			}
			return *this;
		}

		static Matrix CreateTranslation(const Vector3& position) noexcept;
		
		static Matrix CreateRotation(const Vector3& eulerAngles) noexcept;
		
		static Matrix CreateScaling(const Vector3& scale) noexcept;

		static Matrix CreatePerspectiveFovLH(float fov, float aspect, float nearZ, float farZ) noexcept;
		static Matrix CreateOrthographicLH(float width, float height, float nearZ, float farZ) noexcept;

		Vector3 TransformPosition(const Vector3& position) const noexcept;

		Matrix Transposed() noexcept
		{
			DirectX::XMStoreFloat4x4(this, DirectX::XMMatrixTranspose(ToMatrixXM()));
			return *this;
		}

		Matrix Transpose() const noexcept
		{
			Matrix transpose;
			DirectX::XMStoreFloat4x4(&transpose, DirectX::XMMatrixTranspose(ToMatrixXM()));
			return transpose;
		}

		Matrix Inverse() const noexcept 
		{
			Matrix inverse;
			DirectX::XMStoreFloat4x4(&inverse, DirectX::XMMatrixInverse(nullptr, ToMatrixXM()));
			return inverse;
		}

		Vector3 GetTranslation() const noexcept;

		Vector3 GetEulerAngles() const noexcept;

		bool operator==(const Matrix& Other) const noexcept
		{
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					if (m[x][y] != Other.m[x][y]) 
					{
						return false;
					}
				}
			}
			return true;
		}

		bool operator!=(const Matrix& Other) const noexcept
		{
			return !(*this == Other);
		}

		bool Equals(const Matrix& Other, float tolerance = MINUTE_VALUE) noexcept
		{
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					if (fabs(m[x][y] - Other.m[x][y]) > tolerance)
					{
						return false;
					}
				}
			}
			return true;
		}

		DirectX::XMMATRIX ToMatrixXM() const noexcept
		{
			return DirectX::XMLoadFloat4x4(this);
		}

		String ToString() const noexcept
		{
			char buffer[256];
			// 自身のデータを文字列に変換する
			sprintf_s(buffer, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f",
				m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], m[3][2], m[3][3]);

			return buffer;
		}
	};
}