/**
* @file	   IRenderer.h
* @brief
*
* @date	   2022/11/27 2022�N�x����
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
* Renderer���ۃN���X
* ���̃N���X�ł̓C���^�[�t�F�[�X�݂̂������B
* �h���N���X�ŏ��������̊֐�����������B
*/
class IRenderer : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IRenderer, ISubsystem)
public:

	virtual ~IRenderer() = default;

	virtual bool Initialize() override;

public:

	/** Light ���\�b�h */
	void AddLight(ILight* light) noexcept;
	void RemoveLight(ILight* light) noexcept;

	/** Camera ���\�b�h */
	void AddCamera(ICamera* camera) noexcept;
	void RemoveCamera(ICamera* camera) noexcept;
	ICamera* GetMainCamera() const noexcept;

	/** Render Object ���\�b�h */
	void AddRenderObject(IRenderObject* rederObject) noexcept;
	void RemoveRenderObject(IRenderObject* rederObject) noexcept;

	/** Post Process Effect ���\�b�h */
	void RegisterPostProcess(IPostProcessEffect* postProcess) noexcept;
	void UnRegisterPostProcess(IPostProcessEffect* postProcess) noexcept;
	IPostProcessEffect* GetPostProcess() noexcept;
	bool HasPostProcessSetting() noexcept;

	/** �A�N�Z�X */
	SkyBox*			  GetSkyBox()			const noexcept { return m_skyBox.get(); }
	TransformCBuffer* GetTransformCBuffer() const noexcept { return m_transformCBuffer.get(); }

protected:

	// * �X�J�C�{�b�N�X�N���X
	UniquePtr<SkyBox> m_skyBox;

	// * ���C�g�Ǘ��̂��߂̃}�b�v�N���X
	UniquePtr<ILightMap> m_lightMap;

	// * �s��Ǘ��̂��߂̃N���X
	UniquePtr<TransformCBuffer> m_transformCBuffer;

	// * Scene�ɐݒu���ꂽ�`��I�u�W�F�N�g�z��B
	Vector<IRenderObject*> m_renderObjects;

	ICamera* m_mainCamera = nullptr;

	// * �z��ɂ���̂͌�Ƀ��C���[�Ȃǂ�UI�Ȃǂ̕`��𐧌䂳���Ă������߁B
	Vector<ICamera*> m_cameras;

	IPostProcessEffect* m_postProcessEffect = nullptr;
};