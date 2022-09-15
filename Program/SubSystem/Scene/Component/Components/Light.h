/**
* @file    Light.h
* @brief
*
* @date	   2022/09/04 2022�N�x����
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

	void Initialize() override;
	void Remove() override;

	void SetActive(bool active) override;

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

	Renderer* m_renderer;

	LightType m_lightType;

	Math::Vector4 m_color;

	// * ���̋���
	float m_intensity;

	// * ���̓͂��͈́@(�|�C���g�A�X�|�b�g���C�g���̂ݗL��)
	float m_influenceRange;

	// * �X�|�b�g���C�g�̎ˏo�����@(�X�|�b�g���C�g���̂ݗL��)
	float m_angle;
};