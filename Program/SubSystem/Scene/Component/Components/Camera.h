/**
* @file    Camera.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../ICamera.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"

class IRenderer;

class Camera : public ICamera
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Camera, ICamera)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	bool Erasable() override;

public:

	/** view port �̃T�C�Y�ݒ�𔽉f */
	float GetWidth() const override;
	void SetWidth(float width) override;
	float GetHeight() const override;
	void SetHeight(float height) override;

	/** ���������ł� sWidth / Height �ɂ͈ˑ����Ȃ��B*/
	float GetAspect() const override;
	void SetAspect(float aspect) override;

	float GetFov() const override;
	void SetFov(float fov) override;
	float GetNear() const override;
	void SetNear(float nearClip) override;
	float GetFar() const override;
	void SetFar(float farClip) override;

	const Math::Matrix& GetViewMatrix() const override;
	const Math::Matrix& GetProjectionMatrix() const override;
	const Math::Matrix& GetOrthographicMatrix() const override;

private:

	/** �e�ݒ肩��e�v���W�F�N�V�����s����쐬 */
	void CreateProjectionMatrix() noexcept;
	void CreateOrthographicMatrix() noexcept;

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

private:

	IRenderer* m_renderer = nullptr;

	bool m_isRegister = false;

	RenderCommandFance m_renderCommandFance;

	Math::Matrix m_projection;
	Math::Matrix m_orthographic;

	// * �J���� �ݒ�
	float m_aspect;
	float m_width;
	float m_height;
	float m_fov;
	float m_near;
	float m_far;
};