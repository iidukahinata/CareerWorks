/**
* @file	   Renderer.h
* @brief
*
* @date	   2022/10/28 2022�N�x����
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
* Renderer���ۃN���X
* ���̃N���X�ł̓C���^�[�t�F�[�X�݂̂������B
* �h���N���X�ŏ��������̊֐�����������B
*/
class Renderer : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Renderer)
public:

	virtual bool Initialize() override;

public:

	/** Light ���\�b�h */
	void AddLight(Light* light) noexcept;
	void RemoveLight(Light* light) noexcept;

	/** Camera ���\�b�h */
	void AddCamera(Camera* camera) noexcept;
	void RemoveCamera(Camera* camera) noexcept;
	Camera* GetMainCamera() const noexcept;

	/** Render Object ���\�b�h */
	void AddRenderObject(RenderObject* rederObject) noexcept;
	void RemoveRenderObject(RenderObject* rederObject) noexcept;

	/** Post Process Effect ���\�b�h */
	void RegisterPostProcess(PostProcessEffect* postProcess) noexcept;
	void UnRegisterPostProcess(PostProcessEffect* postProcess) noexcept;
	PostProcessEffect* GetPostProcess() noexcept;
	bool HasPostProcessSetting() noexcept;

	/** Deferred Renderer */
	virtual void RegisterGBufferShader(StringView path) {}
	virtual String GetGBufferShader() { return "none"; }

	/** �A�N�Z�X */
	SkyBox*			  GetSkyBox()			const noexcept { return m_skyBox.get(); }
	TransformCBuffer* GetTransformCBuffer() const noexcept { return m_transformCBuffer.get(); }

protected:

	// * �X�J�C�{�b�N�X�N���X
	UniquePtr<SkyBox> m_skyBox;

	// * ���C�g�Ǘ��̂��߂̃}�b�v�N���X
	UniquePtr<LightMap> m_lightMap;

	// * �s��Ǘ��̂��߂̃N���X
	UniquePtr<TransformCBuffer> m_transformCBuffer;

	// * Scene�ɐݒu���ꂽ�`��I�u�W�F�N�g�z��B
	Vector<RenderObject*> m_renderObjects;

	Camera* m_mainCamera = nullptr;

	// * �z��ɂ���̂͌�Ƀ��C���[�Ȃǂ�UI�Ȃǂ̕`��𐧌䂳���Ă������߁B
	Vector<Camera*> m_cameras;

	PostProcessEffect* m_postProcessEffect = nullptr;
};