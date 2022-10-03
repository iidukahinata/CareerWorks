/**
* @file    Camera.h
* @brief
*
* @date	   2022/10/02 2022年度初版
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

	void OnInitialize() override;
	void OnStart() override;
	void OnStop() override;

public:

	/** view port のサイズ設定を反映 */
	float GetWidth() const noexcept;
	void SetWidth(float width) noexcept;
	float GetHeight() const noexcept;
	void SetHeight(float height) noexcept;

	/** 内部処理では sWidth / Height には依存しない。*/
	float GetAspect() const noexcept;
	void SetAspect(float aspect) noexcept;

	float GetFov() const noexcept;
	void SetFov(float fov) noexcept;
	float GetNear() const noexcept;
	void SetNear(float nearClip) noexcept;
	float GetFar() const noexcept;
	void SetFar(float farClip) noexcept;

	const Math::Matrix& GetViewMatrix() const noexcept;
	const Math::Matrix& GetProjectionMatrix() const noexcept;
	const Math::Matrix& GetOrthographicMatrix() const noexcept;

private:

	/** 各設定から各プロジェクション行列を作成 */
	void CreateProjectionMatrix() noexcept;
	void CreateOrthographicMatrix() noexcept;

	void RegisterToRendererSystem() noexcept;
	void UnRegisterFromRedererSystem() noexcept;

private:

	Renderer* m_renderer;

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