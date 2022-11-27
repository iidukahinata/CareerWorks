/**
* @file	   LightMap.cpp
* @brief   Light�Ǘ��N���X
*
* @date	   2022/08/02 2022�N�x����
*/


#include "ILightMap.h"

ILightMap::~ILightMap()
{
	Clear();
}

void ILightMap::Clear() noexcept
{
	m_lights.clear();
	m_lights.shrink_to_fit();
}

void ILightMap::AddLight(Light* light) noexcept
{
	m_lights.push_back(light);
}

void ILightMap::RemoveLight(Light* light) noexcept
{
	std::erase(m_lights, light);
}

void ILightMap::SetAmbient(const Math::Vector4& ambientLight) noexcept
{
	m_ambientLight = ambientLight;
}