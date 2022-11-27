/**
* @file    Light.h
* @brief
*
* @date	   2022/11/04 2022�N�x����
*/
#pragma once


#include "../IComponent.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"

class IRenderer;

enum LightType
{
	DirectionalLight,
	PointLight,
	SpotLight,
};

class Light : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Light)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	bool Erasable() override;

public:

	void SetLightType(LightType lightType) noexcept;
	LightType GetLightType() const noexcept;

	void SetColor(const Math::Vector4& color) noexcept;
	const Math::Vector4& GetColor() const noexcept;

	void SetIntensity(float intensity) noexcept;
	float GetIntensity() const noexcept;

	/** �|�C���g�A�X�|�b�g���C�g���L�� */
	void SetInfluenceRange(float influenceRange) noexcept;
	float GetInfluenceRange() const noexcept;

	/** �X�|�b�g���C�g���̂ݗL�� */
	void SetAngle(float angle) noexcept;
	float GetAngle() const noexcept;

private:

	void RegisterToLightMap() noexcept;
	void UnRegisterFromLightMap() noexcept;

private:

	IRenderer* m_renderer = nullptr;

	bool m_isRegister = false;

	RenderCommandFance m_renderCommandFance;

	LightType m_lightType;

	Math::Vector4 m_color;

	// * ���̋���
	float m_intensity;

	// * ���̓͂��͈́@(�|�C���g�A�X�|�b�g���C�g���̂ݗL��)
	float m_influenceRange;

	// * �X�|�b�g���C�g�̎ˏo�����@(�X�|�b�g���C�g���̂ݗL��)
	float m_angle;
};