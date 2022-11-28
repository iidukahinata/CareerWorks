/**
* @file    ILight.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "IComponent.h"

enum LightType
{
	DirectionalLight,

	PointLight,

	SpotLight,
};

class ILight : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ILight, IComponent)
public:

	virtual void SetLightType(LightType lightType)
	{
		ASSERT(0);
	}
	virtual LightType GetLightType() const
	{
		ASSERT(0); return {};
	}

	virtual void SetColor(const Math::Vector4& color)
	{
		ASSERT(0);
	}
	virtual const Math::Vector4& GetColor() const
	{
		ASSERT(0); return {};
	}

	virtual void SetIntensity(float intensity)
	{
		ASSERT(0);
	}
	virtual float GetIntensity() const
	{
		ASSERT(0); return {};
	}

	/** ポイント、スポットライト時有効 */
	virtual void SetInfluenceRange(float influenceRange)
	{
		ASSERT(0);
	}
	virtual float GetInfluenceRange() const
	{
		ASSERT(0); return {};
	}

	/** スポットライト時のみ有効 */
	virtual void SetAngle(float angle)
	{
		ASSERT(0);
	}
	virtual float GetAngle() const
	{
		ASSERT(0); return {};
	}
};