/**
* @file	   ViewPortWidget.h
* @brief
*
* @date	   2022/10/31 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class IRenderer;

class ViewPortWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** State �ύX Button �Ȃǂ̕\�����s���B*/
	void ShowToolBar() noexcept;

	/** �V�[�� or �Q�[����ʂ̕\�����s���B*/
	void ShowViewPort() noexcept;

	/** Editor Mode ���̂݃M�Y���\�� */
	void Show3DGuizmo(const ImVec2& cursorPos, float imageWidth, float imageHeight) noexcept;

	/** �e State Button ���g�p����Ă��邩�̔��ʂ����₷�����鏈�� */
	bool ShowStateButton(bool state, std::function<bool()> func) noexcept;

private:

	World* m_world = nullptr;

	IRenderer* m_renderer = nullptr;

	// * �Q�[�����s������ێ�
	bool m_isPlay = false;

	// * ���ݐݒ肪�|�[�Y������\��
	bool m_isPouse = false;

	enum class GuizmoMode
	{
		Translate,
		Rotate,
		Scale,
	};

	GuizmoMode m_guizmoMode = GuizmoMode::Translate;
};