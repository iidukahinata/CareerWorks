/**
* @file    Vector4T.h
* @brief
*
* @date	   2022/06/02 2022îNìxèâî≈
* @author  î—íÀózëæ
*/
#pragma once


#include"MathHelper.h"
#include"Vector4.h"

namespace Math
{
	template<class T>
	struct Vector4T
	{
		COMPLETED_DEVELOPMENT()
	public:

		T x, y, z, w;

	public:

		constexpr Vector4T() noexcept : x(0), y(0), z(0), w(0)
		{}

		explicit constexpr Vector4T(T value) noexcept : x(value), y(value), z(value), w(value)
		{}

		constexpr Vector4T(const Vector4T<T>& V) noexcept : x(V.x), y(V.y), z(V.z), w(V.w)
		{}

		constexpr Vector4T(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w)
		{}

		constexpr const T& operator[](int index) const
		{
			ASSERT(0 <= index);
			ASSERT(index < 4);
			return (&x)[index];
		}

		constexpr T& operator[](int index)
		{
			ASSERT(0 <= index);
			ASSERT(index < 4);
			return (&x)[index];
		}

		constexpr Vector4T<T> operator-() const noexcept
		{
			return Vector4T<T>(-x, -y, -z, -w);
		}

		constexpr Vector4T<T> operator+(const Vector4T<T>& V) const noexcept
		{
			return Vector4T<T>(x + V.x, y + V.y, z + V.z, w + V.w);
		}

		constexpr Vector4T<T> operator+(T value) const noexcept
		{
			return Vector4T<T>(x + value, y + value, z + value, w + value);
		}

		constexpr Vector4T<T> operator-(const Vector4T<T>& V) const noexcept
		{
			return Vector4T<T>(x - V.x, y - V.y, z - V.z, w - V.w);
		}

		constexpr Vector4T<T> operator-(T value) const noexcept
		{
			return Vector4T<T>(x - value, y - value, z - value, w - value);
		}

		constexpr Vector4T<T> operator*(const Vector4T<T>& V) const noexcept
		{
			return Vector4T<T>(x * V.x, y * V.y, z * V.z, w * V.w);
		}

		constexpr Vector4T<T> operator*(T value) const noexcept
		{
			return Vector4T<T>(x * value, y * value, z * value, w * value);
		}

		constexpr Vector4T<T> operator/(const Vector4T<T>& V) const
		{
			return Vector4T<T>(x / V.x, y / V.y, z / V.z, w / V.w);
		}

		constexpr Vector4T<T> operator/(T value) const
		{
			return Vector4T<T>(x / value, y / value, z / value, w / value);
		}

		constexpr Vector4T<T> operator+=(const Vector4T<T>& V) noexcept
		{
			x += V.x;
			y += V.y;
			z += V.z;
			w += V.w;
			return *this;
		}

		constexpr Vector4T<T> operator+=(T value) noexcept
		{
			x += value;
			y += value;
			z += value;
			w += value;
			return *this;
		}

		constexpr Vector4T<T> operator-=(const Vector4T<T>& V) noexcept
		{
			x -= V.x;
			y -= V.y;
			z -= V.z;
			w -= V.w;
			return *this;
		}

		constexpr Vector4T<T> operator-=(T value) noexcept
		{
			x -= value;
			y -= value;
			z -= value;
			w -= value;
			return *this;
		}

		constexpr Vector4T<T> operator*=(const Vector4T<T>& V) noexcept
		{
			x *= V.x;
			y *= V.y;
			z *= V.z;
			w *= V.w;
			return *this;
		}

		constexpr Vector4T<T> operator*=(T value) noexcept
		{
			x *= value;
			y *= value;
			z *= value;
			w *= value;
			return *this;
		}

		constexpr Vector4T<T> operator/=(const Vector4T<T>& V)
		{
			x /= V.x;
			y /= V.y;
			z /= V.z;
			w /= V.w;
			return *this;
		}

		constexpr Vector4T<T> operator/=(T value)
		{
			x /= value;
			y /= value;
			z /= value;
			w /= value;
			return *this;
		}

		T Dot() const noexcept
		{
			return x * x + y * y + z * z + w * w;
		}

		T GetDistance(const Vector4T<T>& V) const noexcept
		{
			T disX = x - V.x;
			T disY = y - V.y;
			T disZ = z - V.z;
			T disW = w - V.w;
			return  static_cast<T>(sqrt(disX * disX + disY * disY + disZ * disZ + disW * disW));
		}

		T GetDistanceSquared(const Vector4T<T>& V) const noexcept
		{
			T disX = x - V.x;
			T disY = y - V.y;
			T disZ = z - V.z;
			T disW = w - V.w;
			return  disX * disX + disY * disY + disZ * disZ + disW * disW;
		}

		T GetLength() const noexcept
		{
			return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
		}

		T GetSquaredLength() const noexcept
		{
			return x * x + y * y + z * z + w * w;
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
				w *= invLength;
			}
			x = y = z = w = 0;
		}

		bool operator==(const Vector4T<T>& V) const noexcept
		{
			return x == V.x && y == V.y && z == V.z && w == V.w;
		}

		bool operator!=(const Vector4T<T>& V) const noexcept
		{
			return x != V.x || y != V.y || z != V.z || w != V.w;
		}

		Vector4 ToVector() const noexcept
		{
			return Vector4(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w));
		}
	};

	typedef Vector4T<int> Vector4i;
	typedef Vector4T<float> Vector4f;
	typedef Vector4T<double> Vector4d;
}