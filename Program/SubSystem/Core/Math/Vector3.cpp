/**
* @file    Vector3.cpp
* @brief
*
* @date	   2022/06/01 2022îNìxèâî≈
* @author  î—íÀózëæ
*/


#include"Vector3.h"
#include"Vector4.h"

namespace Math
{
	const Vector3 Vector3::Zero		  ( 0.0f,  0.0f,  0.0f);
	const Vector3 Vector3::One		  ( 1.0f,  1.0f,  1.0f);
	const Vector3 Vector3::Up		  ( 0.0f,  0.0f,  1.0f);
	const Vector3 Vector3::Down		  ( 0.0f,  0.0f, -1.0f);
	const Vector3 Vector3::Right	  ( 0.0f,  1.0f,  0.0f);
	const Vector3 Vector3::Left		  ( 0.0f, -1.0f,  0.0f);
	const Vector3 Vector3::Forward	  ( 1.0f,  0.0f,  0.0f);
	const Vector3 Vector3::BackForward(-1.0f,  0.0f,  0.0f);

	Vector3::Vector3(const Vector4& V) noexcept : DirectX::XMFLOAT3(V.x, V.y, V.z)
	{}
}