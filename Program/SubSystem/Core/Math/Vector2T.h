/**
* @file    Vector2T.h
* @brief
*
* @date	   2022/06/02 2022îNìxèâî≈
*/
#pragma once


#include"MathHelper.h"
#include"Vector2.h"

namespace Math
{
	template<class T>
	struct Vector2T
	{
		COMPLETED_DEVELOPMENT()
	public:

		T x, y;

	public:

		constexpr Vector2T() noexcept : x(0), y(0)
		{}

		explicit constexpr Vector2T(T value) noexcept : x(value), y(value)
		{}

		constexpr Vector2T(const Vector2T<T>& V) noexcept : x(V.x), y(V.y)
		{}

		constexpr Vector2T(T x, T y) noexcept : x(x), y(y)
		{}

		constexpr const T& operator[](int index) const
		{
			ASSERT(0 <= index);
			ASSERT(index < 2);
			return (&x)[index];
		}

		constexpr T& operator[](int index)
		{
			ASSERT(0 <= index);
			ASSERT(index < 2);
			return (&x)[index];
		}

		constexpr Vector2T<T> operator-() const noexcept
		{
			return Vector2T<T>(-x, -y);
		}

		constexpr Vector2T<T> operator+(const Vector2T<T>& V) const noexcept
		{
			return Vector2T<T>(x + V.x, y + V.y);
		}

		constexpr Vector2T<T> operator+(T value) const noexcept
		{
			return Vector2T<T>(x + value, y + value);
		}

		constexpr Vector2T<T> operator-(const Vector2T<T>& V) const noexcept
		{
			return Vector2T<T>(x - V.x, y - V.y);
		}

		constexpr Vector2T<T> operator-(T value) const noexcept
		{
			return Vector2T<T>(x - value, y - value);
		}

		constexpr Vector2T<T> operator*(const Vector2T<T>& V) const noexcept
		{
			return Vector2T<T>(x * V.x, y * V.y);
		}

		constexpr Vector2T<T> operator*(T value) const noexcept
		{
			return Vector2T<T>(x * value, y * value);
		}

		constexpr Vector2T<T> operator/(const Vector2T<T>& V) const
		{
			return Vector2T<T>(x / V.x, y / V.y);
		}

		constexpr Vector2T<T> operator/(T value) const
		{
			return Vector2T<T>(x / value, y / value);
		}

		constexpr Vector2T<T> operator+=(const Vector2T<T>& V) noexcept
		{
			x += V.x;
			y += V.y;
			return *this;
		}

		constexpr Vector2T<T> operator+=(T value) noexcept
		{
			x += value;
			y += value;
			return *this;
		}

		constexpr Vector2T<T> operator-=(const Vector2T<T>& V) noexcept
		{
			x -= V.x;
			y -= V.y;
			return *this;
		}

		constexpr Vector2T<T> operator-=(T value) noexcept
		{
			x -= value;
			y -= value;
			return *this;
		}

		constexpr Vector2T<T> operator*=(const Vector2T<T>& V) noexcept
		{
			x *= V.x;
			y *= V.y;
			return *this;
		}

		constexpr Vector2T<T> operator*=(T value) noexcept
		{
			x *= value;
			y *= value;
			return *this;
		}

		constexpr Vector2T<T> operator/=(const Vector2T<T>& V)
		{
			x /= V.x;
			y /= V.y;
			return *this;
		}

		constexpr Vector2T<T> operator/=(T value)
		{
			x /= value;
			y /= value;
			return *this;
		}

		T Cross(const Vector2T<T>& V)
		{
			return x * V.y - V.x * y;
		}

		T Dot() const noexcept
		{
			return x * x + y * y;
		}

		T GetDistance(const Vector2T<T>& V) const noexcept
		{
			T disX = x - V.x;
			T disY = y - V.y;
			return  static_cast<T>(sqrt(disX * disX + disY * disY));
		}

		T GetDistanceSquared(const Vector2T<T>& V) const noexcept
		{
			T disX = x - V.x;
			T disY = y - V.y;
			return  disX * disX + disY * disY;
		}

		T GetLength() const noexcept
		{
			return static_cast<T>(sqrt(x * x + y * y));
		}

		T GetSquaredLength() const noexcept
		{
			return x * x + y * y;
		}

		void Normalize() noexcept
		{
			T length = GetLength();
			if (length > 0)
			{
				T invLength = length;
				x *= invLength;
				y *= invLength;
			}
			x = y = 0;
		}

		bool operator==(const Vector2T<T>& V) const noexcept
		{
			return x == V.x && y == V.y;
		}

		bool operator!=(const Vector2T<T>& V) const noexcept
		{
			return x != V.x || y != V.y;
		}

		Vector2 ToVector() const noexcept
		{
			return Vector2(static_cast<float>(x), static_cast<float>(y));
		}
	};

	typedef Vector2T<int> Vector2i;
	typedef Vector2T<float> Vector2f;
	typedef Vector2T<double> Vector2d;
}