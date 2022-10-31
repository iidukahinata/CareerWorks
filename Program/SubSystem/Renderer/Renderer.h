/**
* @file	   Renderer.h
* @brief
*
* @date	   2022/10/28 2022年度初版
*/
#pragma once


#include "SkyBox.h"
#include "LightMap.h"
#include "TransformCBuffer.h"
#include "SubSystem/Core/ISubsystem.h"

class Light;
class Camera;
class RenderObject;
class PostProcessEffect;

/**
* Renderer抽象クラス
* このクラスではインターフェースのみを実装。
* 派生クラスで初期化等の関数を実装する。
*/
class Renderer : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Renderer)
public:

	virtual bool Initialize() override;

public:

	/** Light メソッド */
	void AddLight(Light* light) noexcept;
	void RemoveLight(Light* light) noexcept;

	/** Camera メソッド */
	void AddCamera(Camera* camera) noexcept;
	void RemoveCamera(Camera* camera) noexcept;
	Camera* GetMainCamera() const noexcept;

	/** Render Object メソッド */
	void AddRenderObject(RenderObject* rederObject) noexcept;
	void RemoveRenderObject(RenderObject* rederObject) noexcept;

	/** Post Process Effect メソッド */
	void RegisterPostProcess(PostProcessEffect* postProcess) noexcept;
	void UnRegisterPostProcess(PostProcessEffect* postProcess) noexcept;
	PostProcessEffect* GetPostProcess() noexcept;
	bool HasPostProcessSetting() noexcept;

	/** Deferred Renderer */
	virtual void RegisterGBufferShader(StringView path) {}
	virtual String GetGBufferShader() { return "none"; }

	/** アクセス */
	SkyBox*			  GetSkyBox()			const noexcept { return m_skyBox.get(); }
	TransformCBuffer* GetTransformCBuffer() const noexcept { return m_transformCBuffer.get(); }

protected:

	// * スカイボックスクラス
	UniquePtr<SkyBox> m_skyBox;

	// * ライト管理のためのマップクラス
	UniquePtr<LightMap> m_lightMap;

	// * 行列管理のためのクラス
	UniquePtr<TransformCBuffer> m_transformCBuffer;

	// * Sceneに設置された描画オブジェクト配列。
	Vector<RenderObject*> m_renderObjects;

	Camera* m_mainCamera = nullptr;

	// * 配列にするのは後にレイヤーなどでUIなどの描画を制御させていくため。
	Vector<Camera*> m_cameras;

	PostProcessEffect* m_postProcessEffect = nullptr;
};