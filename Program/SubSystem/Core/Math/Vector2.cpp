/**
* @file    Vector2.cpp
* @brief
*
* @date	   2022/06/01 2022îNìxèâî≈
*/


#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"

namespace Math
{
	const Vector2 Vector2::Zero ( 0.0f,  0.0f);
	const Vector2 Vector2::One  ( 1.0f,  1.0f);
	const Vector2 Vector2::Up   ( 0.0f,  1.0f);
	const Vector2 Vector2::Down ( 0.0f, -1.0f);
	const Vector2 Vector2::Right( 1.0f,  0.0f);
	const Vector2 Vector2::Left (-1.0f,  0.0f);

	Vector2::Vector2(const Vector3& V) noexcept : DirectX::XMFLOAT2(V.x, V.y)
	{}

	Vector2::Vector2(const Vector4& V) noexcept : DirectX::XMFLOAT2(V.x, V.y)
	{}
}