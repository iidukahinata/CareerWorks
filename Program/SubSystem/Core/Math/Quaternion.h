/**
* @file    Quatenion.h
* @brief
*
* @date	   2022/06/01 2022年度初版
*/
#pragma once


#include"MathHelper.h"

namespace Math
{
	struct Quaternion
	{
		COMPLETED_DEVELOPMENT()
	public:
		float x;

		float y;

		float z;

		float w;

	public:

		// * Quaternion (0.0f, 0.0f, 0.0f, 1.0f)
		static const Quaternion Identity;

	public:

		Quaternion() noexcept : x(0.f), y(0.f), z(0.f), w(0.f)
		{}

		Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w)
		{}

		Quaternion(float angle, const Math::Vector3& axis)
		{
			const float half = angle * 0.5f;
			const float s = sin(half);
			const float c = cos(half);

			w = c;
			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
		}

		Quaternion operator-() const
		{
			return Quaternion(-x, -y, -z, -w);
		}

		Quaternion operator+(const Quaternion& Q) const noexcept
		{
			return Quaternion(Q.x + x, Q.y + y, Q.z + z, Q.w + w);
		}

		Quaternion operator-(const Quaternion& Q) const noexcept
		{
			return Quaternion(Q.x - x, Q.y - y, Q.z - z, Q.w - w);
		}

		Quaternion operator*(const Quaternion& Q) const noexcept
		{
			return Quaternion(
				 x * Q.w + y * Q.z - z * Q.y + w * Q.x,
				-x * Q.z + y * Q.w + z * Q.x + w * Q.y,
				 x * Q.y - y * Q.x + z * Q.w + w * Q.z,
				-x * Q.x - y * Q.y - z * Q.z + w * Q.w);
		}

		Quaternion operator*(float value) const noexcept
		{
			return Quaternion(value * x, value * y, value * z, value * w);
		}

		Vector3 operator*(const Vector3& V) const noexcept
		{
			const Vector3 Q(x, y, z);
			const Vector3 T = Vector3::Cross(Q, V) * 2.0f;

			// Vのvector3::Cross(Q, T)は逆行列
			return (V + (T * w) + Vector3::Cross(Q, T));
		}

		Quaternion operator+=(const Quaternion& Q) noexcept
		{
			x += Q.x;
			y += Q.y;
			z += Q.z;
			w += Q.w;
			return *this;
		}

		Quaternion operator-=(const Quaternion& Q) noexcept
		{
			x -= Q.x;
			y -= Q.y;
			z -= Q.z;
			w -= Q.w;
			return *this;
		}

		Quaternion operator*=(const Quaternion& Q) noexcept
		{
			x =  x * Q.w + y * Q.z - z * Q.y + w * Q.x;
			y = -x * Q.z + y * Q.w + z * Q.x + w * Q.y;
			z =  x * Q.y - y * Q.x + z * Q.w + w * Q.z;
			w = -x * Q.x - y * Q.y - z * Q.z + w * Q.w;
			return *this;
		}

		Quaternion operator*=(float value) noexcept
		{
			x *= value;
			y *= value;
			z *= value;
			w *= value;
			return *this;
		}

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

		void Normalize() noexcept
		{
			const float squareLength = x * x + y * y + z * z + w * w;
			if (squareLength > 0.0f)
			{
				const auto invLength = (1.0f / sqrt(squareLength));
				x *= invLength;
				y *= invLength;
				z *= invLength;
				w *= invLength;
			}
		}

		Quaternion Normalized() noexcept
		{
			const float squareLength = x * x + y * y + z * z + w * w;
			if (squareLength > 0.0f)
			{
				const auto invLength = (1.0f / sqrt(squareLength));
				return *this * invLength;
			}

			return *this;
		}

		Quaternion Inverse() const noexcept
		{
			return Quaternion(-x, -y, -z, w);
		}

		Vector3 GetEuler() const noexcept
		{
			const float check = 2.0f * (-y * z + w * x);

			if (check < -0.995f)
			{
				float radZ = -atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z));
				return Vector3(-90.0f, 0.0f, ToDegree(radZ));
			}
			if (check > 0.995f)
			{
				float radZ = atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z));
				return Vector3(90.0f, 0.0f, ToDegree(radZ));
			}

			float radX = asinf(check);
			float radY = atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y));
			float radZ = atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z));

			return Vector3(ToDegree(radX), ToDegree(radY), ToDegree(radZ));
		}

		static Quaternion FromEuler(const Vector3& euler) noexcept
		{
			Vector3 angle(ToRadian(euler.x), ToRadian(euler.y), ToRadian(euler.z));

			return FromYawPitchRool(angle);
		}

		static Quaternion FromYawPitchRool(const Vector3& angle) noexcept
		{
			const float halfPitch = angle.x * 0.5f;
			const float halfYaw	  = angle.y * 0.5f;
			const float halfRoll  = angle.z * 0.5f;

			const float pitchSin = sin(halfPitch);
			const float pitchCos = cos(halfPitch);
			const float yawSin   = sin(halfYaw);
			const float yawCos   = cos(halfYaw);
			const float rollSin  = sin(halfRoll);
			const float rollCos  = cos(halfRoll);

			// オイラー角からQuaternionに変換
			Quaternion quaternion;
			quaternion.x = yawCos * pitchSin * rollCos + yawSin * pitchCos * rollSin;
			quaternion.y = yawSin * pitchCos * rollCos - yawCos * pitchSin * rollSin;
			quaternion.z = yawCos * pitchCos * rollSin - yawSin * pitchSin * rollCos;
			quaternion.w = yawCos * pitchCos * rollCos + yawSin * pitchSin * rollSin;

			return quaternion;
		}

		float Dot(const Quaternion& Q) noexcept
		{
			return x * Q.x + y * Q.y + z * Q.z + w * Q.w;
		}

		bool operator==(const Quaternion& Q) const noexcept
		{
			return x == Q.x && y == Q.y && z == Q.z && w == Q.w;
		}

		bool operator!=(const Quaternion& Q) const noexcept
		{
			return x != Q.x && y != Q.y && z != Q.z && w != Q.w;
		}

		bool Equals(const Quaternion& Q, float tolerance = MINUTE_VALUE) const noexcept
		{
			return Equals(*this, Q, tolerance);
		}

		static bool Equals(const Quaternion& A, const Quaternion& B, float tolerance = MINUTE_VALUE) noexcept
		{
			auto dis = A - B;
			auto add = A + B;

			// 値が反対の行列の場合、も等価のため＋ー二つの判定をとる
			return (fabs(dis.x) <= tolerance && fabs(dis.y) <= tolerance && fabs(dis.z) <= tolerance && fabs(dis.w) <= tolerance)
				|| (fabs(add.x) <= tolerance && fabs(add.y) <= tolerance && fabs(add.z) <= tolerance && fabs(add.w) <= tolerance);
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