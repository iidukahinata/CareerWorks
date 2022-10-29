/**
* @file	   Quad.h
* @brief
*
* @date	   2022/10/28 2022�N�x����
*/
#pragma once


#include "../Geometry/TriangleList.h"

class Cube
{
public:

	template<class T>
	static TriangleList<T> Create(float size = 0.5f) noexcept
	{
		TriangleList<T> triangleList;
		auto& vertices = triangleList.m_vertices;
		auto& indices  = triangleList.m_indices;

		vertices.resize(8);

		vertices[0].position = Math::Vector3(-size,  size, -size);
		vertices[1].position = Math::Vector3( size,  size, -size);
		vertices[2].position = Math::Vector3( size, -size, -size);
		vertices[3].position = Math::Vector3(-size, -size, -size);
		vertices[4].position = Math::Vector3(-size,  size,  size);
		vertices[5].position = Math::Vector3( size,  size,  size);
		vertices[6].position = Math::Vector3( size, -size,  size);
		vertices[7].position = Math::Vector3(-size, -size,  size);

		indices = {
			0, 1, 2,  2, 3, 0,// �O
			5, 4, 7,  7, 6, 5,// ��
			4, 0, 7,  0, 3, 7,// ��
			1, 5, 6,  1, 6, 2,// �E
			0, 4, 1,  4, 5, 1,// ��
			3, 2, 7,  6, 7, 2 // ��
		};

		return triangleList;
	}
};