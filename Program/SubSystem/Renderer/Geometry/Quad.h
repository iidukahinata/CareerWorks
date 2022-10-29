/**
* @file	   Quad.h
* @brief
*
* @date	   2022/10/28 2022îNìxèâî≈
*/
#pragma once


#include "../Geometry/TriangleList.h"

class Quad
{
public:

	template<class T>
	static TriangleList<T> Create(float width = 1.0f, float height = 1.0f) noexcept
	{
		TriangleList<T> triangleList;
		auto& vertices = triangleList.m_vertices;
		auto& indices  = triangleList.m_indices;

		const Math::Vector2 halfSize(width / 2.f, height / 2.f);

		vertices.resize(4);

		vertices[0].position = Math::Vector3( halfSize.x,  halfSize.y, 0.f);
		vertices[1].position = Math::Vector3(-halfSize.x,  halfSize.y, 0.f);
		vertices[2].position = Math::Vector3(-halfSize.x, -halfSize.y, 0.f);
		vertices[3].position = Math::Vector3( halfSize.x, -halfSize.y, 0.f);

		vertices[0].tex = Math::Vector2(1, 0);
		vertices[1].tex = Math::Vector2(0, 0);
		vertices[2].tex = Math::Vector2(0, 1);
		vertices[3].tex = Math::Vector2(1, 1);

		indices = { 0, 1, 2, 2, 3, 0 };

		return triangleList;
	}
};