/**
* @file	   LightMap.cpp
* @brief   Light�Ǘ��N���X
*
* @date	   2022/08/02 2022�N�x����
*/


#include "LightMap.h"

LightMap::~LightMap()
{
	Clear();
}

void LightMap::Clear() noexcept
{
	m_lights.clear();
	m_lights.shrink_to_fit();
}

void LightMap::AddLight(Light* light) noexcept
{
	m_lights.push_back(light);
}

void LightMap::RemoveLight(Light* light) noexcept
{
	std::erase(m_lights, light);
}

void LightMap::SetAmbient(const Math::Vector4& ambientLight) noexcept
{
	m_ambientLight = ambientLight;
}