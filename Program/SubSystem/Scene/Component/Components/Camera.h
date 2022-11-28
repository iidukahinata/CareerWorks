/**
* @file    Camera.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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

	/** view port のサイズ設定を反映 */
	float GetWidth() const override;
	void SetWidth(float width) override;
	float GetHeight() const override;
	void SetHeight(float height) override;

	/** 内部処理では sWidth / Height には依存しない。*/
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

	/** 各設定から各プロジェクション行列を作成 */
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

	// * カメラ 設定
	float m_aspect;
	float m_width;
	float m_height;
	float m_fov;
	float m_near;
	float m_far;
};