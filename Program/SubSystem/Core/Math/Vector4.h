/**
* @file    Vector4.h
* @brief
*
* @date	   2022/06/01 2022年度初版
*/
#pragma once


#include"MathHelper.h"
#include"Vector2.h"
#include"Vector3.h"

namespace Math
{
	struct Vector4 : public DirectX::XMFLOAT4
	{
		COMPLETED_DEVELOPMENT()
	public:

		constexpr Vector4() noexcept : DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f)
		{}

		explicit constexpr Vector4(float value) noexcept : DirectX::XMFLOAT4(value, value, value, value)
		{}

		constexpr Vector4(const Vector4& V) noexcept : DirectX::XMFLOAT4(V.x, V.y, V.z, V.w)
		{}

		explicit constexpr Vector4(const Vector3& V, float w) noexcept : DirectX::XMFLOAT4(V.x, V.y, V.z, w)
		{}

		explicit constexpr Vector4(const Vector2& V, float z, float w) noexcept : DirectX::XMFLOAT4(V.x, V.y, z, w)
		{}

		constexpr Vector4(float x, float y, float z, float w) noexcept : DirectX::XMFLOAT4(x, y, z, w)
		{}

		const float& operator[](int index) const
		{
			ASSERT(0 <= index);
			ASSERT(index < 4);
			return (&x)[index];
		}

		float& operator[](int index)
		{
			ASSERT(0 <= index);
			ASSERT(index < 4);
			return (&x)[index];
		}

		constexpr Vector4 operator-() const noexcept
		{
			return Vector4(-x, -y, -z, -w);
		}

		constexpr Vector4 operator+(const Vector4& V) const noexcept
		{
			return Vector4(x + V.x, y + V.y, z + V.z, w + V.w);
		}

		constexpr Vector4 operator+(float value) const noexcept
		{
			return Vector4(x + value, y + value, z + value, w + value);
		}

		constexpr Vector4 operator-(const Vector4& V) const noexcept
		{
			return Vector4(x - V.x, y - V.y, z - V.z, w - V.w);
		}

		constexpr Vector4 operator-(float value) const noexcept
		{
			return Vector4(x - value, y - value, z - value, w - value);
		}

		constexpr Vector4 operator*(const Vector4& V) const noexcept
		{
			return Vector4(x * V.x, y * V.y, z * V.z, w * V.w);
		}

		constexpr Vector4 operator*(float value) const noexcept
		{
			return Vector4(x * value, y * value, z * value, w * value);
		}

		constexpr Vector4 operator/(const Vector4& V) const
		{
			return Vector4(x / V.x, y / V.y, z / V.z, w / V.w);
		}

		constexpr Vector4 operator/(float value) const
		{
			return Vector4(x / value, y / value, z / value, w / value);
		}

		constexpr Vector4 operator+=(const Vector4& V) noexcept
		{
			x += V.x;
			y += V.y;
			z += V.z;
			w += V.w;
			return *this;
		}

		constexpr Vector4 operator+=(float value) noexcept
		{
			x += value;
			y += value;
			z += value;
			w += value;
			return *this;
		}

		constexpr Vector4 operator-=(const Vector4& V) noexcept
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;
			w -= V.w;
			return *this;
		}

		Vector4 operator-=(float value) noexcept
		{
			x -= value;
			y -= value;
			z -= value;
			w -= value;
			return *this;
		}

		constexpr Vector4 operator*=(const Vector4& V) noexcept
		{
			x *= V.x;
			y *= V.y;
			z *= V.z;
			w *= V.w;
			return *this;
		}

		constexpr Vector4 operator*=(float value) noexcept
		{
			x *= value;
			y *= value;
			z *= value;
			w *= value;
			return *this;
		}

		constexpr Vector4 operator/=(const Vector4& V)
		{
			x /= V.x;
			y /= V.y;
			z /= V.z;
			w /= V.w;
			return *this;
		}

		constexpr Vector4 operator/=(float value)
		{
			x /= value;
			y /= value;
			z /= value;
			w /= value;
			return *this;
		}

		float Dot(const Vector4& V) const noexcept
		{
			float result;
			auto v = DirectX::XMVector4Dot(DirectX::XMLoadFloat4(this), DirectX::XMLoadFloat4(&V));
			DirectX::XMStoreFloat(&result, std::move(v));
			return result;
		}

		void Normalize() noexcept
		{
			DirectX::XMStoreFloat4(this, DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(this)));
		}

		float GetLenght() const noexcept
		{
			float result;
			DirectX::XMStoreFloat(&result, DirectX::XMVector4Length(DirectX::XMLoadFloat4(this)));
			return result;
		}

		float GetSquaredLength() const noexcept
		{
			return x * x + y * y + z * z + w * w;
		}

		float Distance(const Vector4& V) const noexcept
		{
			return (*this - V).GetLenght();
		}

		float DistanceSquared(const Vector3& V) const noexcept
		{
			return (Vector3(*this) - V).GetSquaredLength();
		}

		bool operator==(const Vector4& V) const noexcept
		{
			return x == V.x && y == V.y && z == V.z && w == V.w;
		}

		bool operator!=(const Vector4& V) const noexcept
		{
			return x != V.x || y != V.y || z != V.z || w != V.w;
		}

		bool Equals(const Vector4& V, float tolerance = MINUTE_VALUE) const noexcept
		{
			auto dis = *this - V;
			return fabs(dis.x) <= tolerance && fabs(dis.y) <= tolerance
				&& fabs(dis.z) <= tolerance && fabs(dis.w) <= tolerance;
		}

		String ToString() const noexcept
		{
			char buffer[128];
			// 自身のデータを文字列に変換する
			sprintf_s(buffer, "X:%f, Y:%f, Z:%f, W:%f", x, y, z, w);
			return buffer;
		}
	};
}