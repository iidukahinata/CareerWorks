/**
* @file    Vector3T.h
* @brief
*
* @date	   2022/06/02 2022îNìxèâî≈
*/
#pragma once


#include"MathHelper.h"
#include"Vector3.h"

namespace Math
{
	template<class T>
	struct Vector3T
	{
		COMPLETED_DEVELOPMENT()
	public:

		T x, y, z;

	public:

		constexpr Vector3T() noexcept : x(0), y(0), z(0)
		{}

		explicit constexpr Vector3T(T value) noexcept : x(value), y(value), z(value)
		{}

		constexpr Vector3T(const Vector3T<T>& V) noexcept : x(V.x), y(V.y), z(V.z)
		{}

		constexpr Vector3T(T x, T y, T z) noexcept : x(x), y(y), z(z)
		{}

		constexpr const T& operator[](int index) const
		{
			ASSERT(0 <= index);
			ASSERT(index < 3);
			return (&x)[index];
		}

		constexpr T& operator[](int index)
		{
			ASSERT(0 <= index);
			ASSERT(index < 3);
			return (&x)[index];
		}

		constexpr Vector3T<T> operator-() const noexcept
		{
			return Vector3T<T>(-x, -y, -z);
		}

		constexpr Vector3T<T> operator+(const Vector3T<T>& V) const noexcept
		{
			return Vector3T<T>(x + V.x, y + V.y, z + V.z);
		}

		constexpr Vector3T<T> operator+(T value) const noexcept
		{
			return Vector3T<T>(x + value, y + value, z + value);
		}

		constexpr Vector3T<T> operator-(const Vector3T<T>& V) const noexcept
		{
			return Vector3T<T>(x - V.x, y - V.y, z - V.z);
		}

		constexpr Vector3T<T> operator-(T value) const noexcept
		{
			return Vector3T<T>(x - value, y - value, z - value);
		}

		constexpr Vector3T<T> operator*(const Vector3T<T>& V) const noexcept
		{
			return Vector3T<T>(x * V.x, y * V.y, z * V.z);
		}

		constexpr Vector3T<T> operator*(T value) const noexcept
		{
			return Vector3T<T>(x * value, y * value, z * value);
		}

		constexpr Vector3T<T> operator/(const Vector3T<T>& V) const
		{
			return Vector3T<T>(x / V.x, y / V.y, z / V.z);
		}

		constexpr Vector3T<T> operator/(T value) const
		{
			return Vector3T<T>(x / value, y / value, z / value);
		}

		constexpr Vector3T<T> operator+=(const Vector3T<T>& V) noexcept
		{
			x += V.x;
			y += V.y;
			z += V.z;
			return *this;
		}

		constexpr Vector3T<T> operator+=(T value) noexcept
		{
			x += value;
			y += value;
			z += value;
			return *this;
		}

		constexpr Vector3T<T> operator-=(const Vector3T<T>& V) noexcept
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;
			return *this;
		}

		constexpr Vector3T<T> operator-=(T value) noexcept
		{
			x -= value;
			y -= value;
			z -= value;
			return *this;
		}

		constexpr Vector3T<T> operator*=(const Vector3T<T>& V) noexcept
		{
			x *= V.x;
			y *= V.y;
			z *= V.z;
			return *this;
		}

		constexpr Vector3T<T> operator*=(T value) noexcept
		{
			x *= value;
			y *= value;
			z *= value;
			return *this;
		}

		constexpr Vector3T<T> operator/=(const Vector3T<T>& V)
		{
			x /= V.x;
			y /= V.y;
			z /= V.z;
			return *this;
		}

		constexpr Vector3T<T> operator/=(T value)
		{
			x /= value;
			y /= value;
			z /= value;
			return *this;
		}

		Vector3T<T> Cross(const Vector3T<T>& V)
		{
			return Vector3T<T>(y * V.z - z * V.y, z * V.x - x * V.z, x * V.y - y * V.x);
		}

		T Dot() const noexcept
		{
			return x * x + y * y + z * z;
		}

		T GetDistance(const Vector3T<T>& V) const noexcept
		{
			T disX = x - V.x;
			T disY = y - V.y;
			T disZ = z - V.z;
			return  static_cast<T>(sqrt(disX * disX + disY * disY + disZ * disZ));
		}

		T GetDistanceSquared(const Vector3T<T>& V) const noexcept
		{
			T disX = x - V.x;
			T disY = y - V.y;
			T disZ = z - V.z;
			return  disX * disX + disY * disY + disZ * disZ;
		}

		T GetLength() const noexcept
		{
			return static_cast<T>(sqrt(x * x + y * y + z * z));
		}

		T GetSquaredLength() const noexcept
		{
			return x * x + y * y + z * z;
		}

		void Normalize() noexcept
		{
			T length = GetLength();
			if (length > 0)
			{
				T invLength = length;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
			x = y = z = 0;
		}

		bool operator==(const Vector3T<T>& V) const noexcept
		{
			return x == V.x && y == V.y && z == V.z;
		}

		bool operator!=(const Vector3T<T>& V) const noexcept
		{
			return x != V.x || y != V.y || z != V.z;
		}

		Vector3 ToVector() const noexcept
		{
			return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
		}
	};

	typedef Vector3T<int> Vector3i;
	typedef Vector3T<float> Vector3f;
	typedef Vector3T<double> Vector3d;
}