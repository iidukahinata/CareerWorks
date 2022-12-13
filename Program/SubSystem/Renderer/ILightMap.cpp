/**
* @file	   ILightMap.cpp
*
* @date	   2022/11/28 2022îNìxèâî≈
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

void ILightMap::AddLight(ILight* light) noexcept
{
	m_lights.push_back(light);
}

void ILightMap::RemoveLight(ILight* light) noexcept
{
	std::erase(m_lights, light);
}

void ILightMap::SetAmbient(const Math::Vector4& ambientLight) noexcept
{
	m_ambientLight = ambientLight;
}

const Math::Vector4& ILightMap::GetAmbient() const noexcept
{
	return m_ambientLight;
}