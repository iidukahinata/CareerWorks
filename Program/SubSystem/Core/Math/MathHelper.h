/**
* @file    MathHelper.h
* @brief
*
* @date	   2022/07/06 2022îNìxèâî≈
*/
#pragma once


#include <DirectXMath.h>

#define PI 3.14159265359f
#define PI_2 6.28318530718f
#define PI_4 12.5663706144f
#define VERY_SMALL_VALUE 1.0e-8f
#define MINUTE_VALUE 1.0e-4f

namespace Math
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix;

	constexpr float ToRadian(float degree) noexcept
	{
		constexpr float rad = PI / 180.f;
		return degree * rad;
	}

	constexpr float ToDegree(float radian) noexcept
	{
		constexpr float degree = 180.f / PI;
		return radian * degree;
	}
}