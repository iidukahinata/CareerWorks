/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/09/02 2022�N�x����
*/
#pragma once


#include "../Renderer.h"

class ForwardRenderer : public Renderer
{
	IN_DEVELOPMENT("�ʃX���b�h��ł̓�����J���\�� & LightMap �����҂�")
	SUB_CLASS(ForwardRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	void Update() noexcept;
	void Present() noexcept;

private:

	// * Render �� Present
	Array<Job, 2> m_jobs;
};