/**
* @file    Camera.h
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/
#pragma once


#include "../IComponent.h"

class Renderer;

class Camera : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Camera)
public:

	void Serialized(FileStream* file) const override;
	void Deserialization(FileStream* file) override;

	void Initialize() override;
	void Remove() override;

	void SetActive(bool active) override;

public:

	/** view port �̃T�C�Y�ݒ�𔽉f */
	float GetWidth() const;
	void SetWidth(float width);
	float GetHeight() const;
	void SetHeight(float height);

	/** ���������ł� sWidth / Height �ɂ͈ˑ����Ȃ��B*/
	float GetAspect() const;
	void SetAspect(float aspect);

	float GetFov() const;
	void SetFov(float fov);
	float GetNear() const;
	void SetNear(float nearClip);
	float GetFar() const;
	void SetFar(float farClip);

	const Math::Matrix& GetViewMatrix();
	const Math::Matrix& GetProjectionMatrix();
	const Math::Matrix& GetOrthographicMatrix();

private:

	/** �e�ݒ肩��e�v���W�F�N�V�����s����쐬 */
	void CreateProjectionMatrix();
	void CreateOrthographicMatrix();

	void RegisterToRendererSystem();
	void UnRegisterFromRedererSystem();

private:

	Renderer* m_renderer;

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