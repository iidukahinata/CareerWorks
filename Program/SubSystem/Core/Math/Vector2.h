/**
* @file    Vector2.h
* @brief
*
* @date	   2022/06/01 2022?N?x????
*/
#pragma once


#include"MathHelper.h"

namespace Math
{
	struct Vector2 : public DirectX::XMFLOAT2
	{
		COMPLETED_DEVELOPMENT()
	public:

		// * vector (0.0f, 0.0f)
		static const Vector2 Zero;

		// * vector ( 1.0f, 1.0f)
		static const Vector2 One;

		// * vector ( 0.0f, 1.0f)
		static const Vector2 Up;

		// * vector ( 0.0f,-1.0f)
		static const Vector2 Down;

		// * vector ( 1.0f, 0.0f)
		static const Vector2 Right;

		// * vector (-1.0f, 0.0f)
		static const Vector2 Left;

	public:

		constexpr Vector2() noexcept : DirectX::XMFLOAT2(0, 0)
		{}

		explicit constexpr Vector2(float value) noexcept : DirectX::XMFLOAT2(value, value)
		{}

		explicit Vector2(const Vector4& V) noexcept;

		explicit Vector2(const Vector3& V) noexcept;

		constexpr Vector2(const Vector2& V) noexcept : DirectX::XMFLOAT2(V.x, V.y)
		{}

		constexpr Vector2(float x, float y) noexcept : DirectX::XMFLOAT2(x, y)
		{}

		constexpr const float& operator[](int index) const
		{
			ASSERT(0 <= index);
			ASSERT(index < 2);
			return (&x)[index];
		}

		constexpr float& operator[](int index)
		{
			ASSERT(0 <= index);
			ASSERT(index < 2);
			return (&x)[index];
		}

		constexpr Vector2 operator-() const noexcept
		{
			return Vector2(-x, -y);
		}

		constexpr Vector2 operator+(const Vector2& V) const noexcept
		{
			return Vector2(x + V.x, y + V.y);
		}

		constexpr Vector2 operator+(float value) const noexcept
		{
			return Vector2(x + value, y + value);
		}

		constexpr Vector2 operator-(const Vector2& V) const noexcept
		{
			return Vector2(x - V.x, y - V.y);
		}

		constexpr Vector2 operator-(float value) const noexcept
		{
			return Vector2(x - value, y - value);
		}

		constexpr Vector2 operator*(const Vector2& V) const noexcept
		{
			return Vector2(x * V.x, y * V.y);
		}

		constexpr Vector2 operator*(float value) const noexcept
		{
			return Vector2(x * value, y * value);
		}

		constexpr Vector2 operator/(const Vector2& V) const
		{
			return Vector2(x / V.x, y / V.y);
		}

		constexpr Vector2 operator/(float value) const
		{
			return Vector2(x / value, y / value);
		}

		constexpr Vector2 operator+=(const Vector2& V) noexcept
		{
			x += V.x;
			y += V.y;
			return *this;
		}

		constexpr Vector2 operator+=(float value) noexcept
		{
			x += value;
			y += value;
			return *this;
		}

		constexpr Vector2 operator-=(const Vector2& V) noexcept
		{
			x -= V.x;
			y -= V.y;
			return *this;
		}

		constexpr Vector2 operator-=(float value) noexcept
		{
			x -= value;
			y -= value;
			return *this;
		}

		constexpr Vector2 operator*=(const Vector2& V) noexcept
		{
			x *= V.x;
			y *= V.y;
			return *this;
		}

		constexpr Vector2 operator*=(float value) noexcept
		{
			x *= value;
			y *= value;
			return *this;
		}

		constexpr Vector2 operator/=(const Vector2& V)
		{
			x /= V.x;
			y /= V.y;
			return *this;
		}

		constexpr Vector2 operator/=(float value)
		{
			x /= value;
			y /= value;
			return *this;
		}

		Vector2 Lerp(const Vector2& V, float t) noexcept
		{
			return *this + ((V - *this) * t);
		}

		Vector2 Slerp(const Vector2& V, float t) noexcept
		{
			auto angle = this->Dot(V);

			auto p0 = *this * sin(angle * (1.f - t));
			auto p1 = V		* sin(angle * t);

			return (p0 + p1) / sin(angle);
		}

		float Cross(const Vector2& V) const noexcept
		{
			float result;
			auto v = DirectX::XMVector2Cross(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&V));
			DirectX::XMStoreFloat(&result, std::move(v));
			return result;
		}

		float Dot(const Vector2& V) const noexcept
		{
			float result;
			auto v = DirectX::XMVector2Dot(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&V));
			DirectX::XMStoreFloat(&result, std::move(v));
			return result;
		}

		void Normalize() noexcept
		{
			DirectX::XMStoreFloat2(this, DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(this)));
		}

		float GetLenght() const noexcept
		{
			float result;
			DirectX::XMStoreFloat(&result, DirectX::XMVector2Length(DirectX::XMLoadFloat2(this)));
			return result;
		}

		float GetSquaredLength() const  noexcept
		{
			return x * x + y * y;
		}

		float Distance(const Vector2& V) const noexcept
		{
			return (*this - V).GetLenght();
		}

		float DistanceSquared(const Vector2& V) const noexcept
		{
			return (*this - V).GetSquaredLength();
		}

		bool operator==(const Vector2& V) const noexcept
		{
			return x == V.x && y == V.y;
		}

		bool operator!=(const Vector2& V) const noexcept
		{
			return x != V.x || y != V.y;
		}

		bool Equals(const Vector2& V, float tolerance = MINUTE_VALUE) const noexcept
		{
			auto dis = *this - V;
			return fabs(dis.x) <= tolerance && fabs(dis.y) <= tolerance;
		}

		String ToString() const noexcept
		{
			char buffer[128];
			// ???g?̃f?[?^?𕶎????ɕϊ?????
			sprintf_s(buffer, "X:%f, Y:%f", x, y);
			return buffer;
		}
	};
}