/**
* @file    Light.h
* @brief
*
* @date	   2022/10/03 2022年度初版
*/
#pragma once


#include "../IComponent.h"

class Renderer;

enum class LightType
{
	DirectionalLight,
	PointLight,
	SpotLight,
	Max,
};

class Light : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Light)
public:

	void Serialized(FileStream* file) const override;
	void Deserialization(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;

public:

	void SetLightType(LightType lightType) noexcept;
	LightType GetLightType() const noexcept;

	void SetColor(const Math::Vector4& color) noexcept;
	const Math::Vector4& GetColor() const noexcept;

	void SetIntensity(float intensity) noexcept;
	float GetIntensity() const noexcept;

	/** ポイント、スポットライト時有効 */
	void SetInfluenceRange(float influenceRange) noexcept;
	float GetInfluenceRange() const noexcept;

	/** スポットライト時のみ有効 */
	void SetAngle(float angle) noexcept;
	float GetAngle() const noexcept;

private:

	void RegisterToLightMap() noexcept;
	void UnRegisterFromLightMap() noexcept;

private:

	Renderer* m_renderer = nullptr;

	LightType m_lightType;

	Math::Vector4 m_color;

	// * 光の強さ
	float m_intensity;

	// * 光の届く範囲　(ポイント、スポットライト時のみ有効)
	float m_influenceRange;

	// * スポットライトの射出方向　(スポットライト時のみ有効)
	float m_angle;
};