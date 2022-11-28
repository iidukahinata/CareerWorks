/**
* @file	   IRenderer.h
* @brief
*
* @date	   2022/11/27 2022年度初版
*/
#pragma once


#include "SkyBox.h"
#include "ILightMap.h"
#include "TransformCBuffer.h"
#include "SubSystem/Core/ISubsystem.h"

class ILight;
class ICamera;
class IRenderObject;
class IPostProcessEffect;

/**
* Renderer抽象クラス
* このクラスではインターフェースのみを実装。
* 派生クラスで初期化等の関数を実装する。
*/
class IRenderer : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IRenderer, ISubsystem)
public:

	virtual ~IRenderer() = default;

	virtual bool Initialize() override;

public:

	/** Light メソッド */
	void AddLight(ILight* light) noexcept;
	void RemoveLight(ILight* light) noexcept;

	/** Camera メソッド */
	void AddCamera(ICamera* camera) noexcept;
	void RemoveCamera(ICamera* camera) noexcept;
	ICamera* GetMainCamera() const noexcept;

	/** Render Object メソッド */
	void AddRenderObject(IRenderObject* rederObject) noexcept;
	void RemoveRenderObject(IRenderObject* rederObject) noexcept;

	/** Post Process Effect メソッド */
	void RegisterPostProcess(IPostProcessEffect* postProcess) noexcept;
	void UnRegisterPostProcess(IPostProcessEffect* postProcess) noexcept;
	IPostProcessEffect* GetPostProcess() noexcept;
	bool HasPostProcessSetting() noexcept;

	/** アクセス */
	SkyBox*			  GetSkyBox()			const noexcept { return m_skyBox.get(); }
	TransformCBuffer* GetTransformCBuffer() const noexcept { return m_transformCBuffer.get(); }

protected:

	// * スカイボックスクラス
	UniquePtr<SkyBox> m_skyBox;

	// * ライト管理のためのマップクラス
	UniquePtr<ILightMap> m_lightMap;

	// * 行列管理のためのクラス
	UniquePtr<TransformCBuffer> m_transformCBuffer;

	// * Sceneに設置された描画オブジェクト配列。
	Vector<IRenderObject*> m_renderObjects;

	ICamera* m_mainCamera = nullptr;

	// * 配列にするのは後にレイヤーなどでUIなどの描画を制御させていくため。
	Vector<ICamera*> m_cameras;

	IPostProcessEffect* m_postProcessEffect = nullptr;
};