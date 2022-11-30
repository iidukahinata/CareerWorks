/**
* @file	   Quad.h
* @brief
*
* @date	   2022/10/28 2022年度初版
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

		vertices[0].position = Math::Vector3(-size,  size,  size);
		vertices[1].position = Math::Vector3( size,  size,  size);
		vertices[2].position = Math::Vector3(-size,  size, -size);
		vertices[3].position = Math::Vector3( size,  size, -size);
		vertices[4].position = Math::Vector3(-size, -size,  size);
		vertices[5].position = Math::Vector3( size, -size,  size);
		vertices[6].position = Math::Vector3(-size, -size, -size);
		vertices[7].position = Math::Vector3( size, -size, -size);

		indices = {
			2, 1, 0,  1, 3, 2,// 前
			4, 0, 2,  2, 6, 4,// 奥
			5, 1, 0,  0, 4, 5,// 左
			7, 3, 1,  1, 5, 7,// 右
			6, 2, 3,  3, 7, 6,// 上
			4, 6, 7,  7, 5, 4 // 下
		};

		return triangleList;
	}
};