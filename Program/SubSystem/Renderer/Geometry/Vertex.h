/**
* @file	   Vertex.h
* @brief
*
* @date	   2022/08/07 2022îNìxèâî≈
*/
#pragma once


struct VertexPos
{
	Math::Vector3 position;
};

struct Vertex2D
{
	Math::Vector3 position;
	Math::Vector2 tex;
};

struct Vertex3D
{
	Math::Vector3 position;
	Math::Vector2 tex;
	Math::Vector3 normal;
};

struct VertexBump3D
{
	Math::Vector3 position;
	Math::Vector2 tex;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector3 biNormal;
};