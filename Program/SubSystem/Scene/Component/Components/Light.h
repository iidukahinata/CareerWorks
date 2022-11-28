/**
* @file    Light.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../ILight.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"

class IRenderer;

class Light : public ILight
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Light, ILight)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	bool Erasable() override;

public:

	void SetLightType(LightType lightType) override;
	LightType GetLightType() const override;

	void SetColor(const Math::Vector4& color) override;
	const Math::Vector4& GetColor() const override;

	void SetIntensity(float intensity) override;
	float GetIntensity() const override;

	/** ポイント、スポットライト時有効 */
	void SetInfluenceRange(float influenceRange) override;
	float GetInfluenceRange() const override;

	/** スポットライト時のみ有効 */
	void SetAngle(float angle) override;
	float GetAngle() const override;

private:

	void RegisterToLightMap() noexcept;
	void UnRegisterFromLightMap() noexcept;

private:

	IRenderer* m_renderer = nullptr;

	bool m_isRegister = false;

	RenderCommandFance m_renderCommandFance;

	LightType m_lightType;

	Math::Vector4 m_color;

	// * 光の強さ
	float m_intensity;

	// * 光の届く範囲　(ポイント、スポットライト時のみ有効)
	float m_influenceRange;

	// * スポットライトの射出方向　(スポットライト時のみ有効)
	float m_angle;
};