/**
* @file	   TriangleList.h
* @brief
*
* @date	   2022/09/02 2022”N“x‰”Å
*/
#pragma once


template<class T>
struct TriangleList
{
	Vector<T> m_vertices;
	Vector<UINT> m_indices;

public:

	TriangleList() = default;
	TriangleList(std::vector<T> vertices, std::vector<unsigned short> indices) :
		m_vertices(vertices), m_indices(indices)
	{

	}
};