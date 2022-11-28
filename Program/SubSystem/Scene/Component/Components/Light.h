/**
* @file    Light.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
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

	/** �|�C���g�A�X�|�b�g���C�g���L�� */
	void SetInfluenceRange(float influenceRange) override;
	float GetInfluenceRange() const override;

	/** �X�|�b�g���C�g���̂ݗL�� */
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

	// * ���̋���
	float m_intensity;

	// * ���̓͂��͈́@(�|�C���g�A�X�|�b�g���C�g���̂ݗL��)
	float m_influenceRange;

	// * �X�|�b�g���C�g�̎ˏo�����@(�X�|�b�g���C�g���̂ݗL��)
	float m_angle;
};