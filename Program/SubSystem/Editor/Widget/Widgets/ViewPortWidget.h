/**
* @file	   ViewPortWidget.h
* @brief
*
* @date	   2022/10/25 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;

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

	/** �e State Button ���g�p����Ă��邩�̔��ʂ����₷���ׂɊ֐��� */
	bool ShowStateButton(bool state, std::function<bool()> func) noexcept;

private:

	World* m_world = nullptr;

	// * �Q�[�����s������ێ�
	bool m_isPlay = false;

	// * ���ݐݒ肪�|�[�Y������\��
	bool m_isPouse = false;
};