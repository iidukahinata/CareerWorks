/**
* @file	   ForwardRenderer.h
* @brief
*
* @date	   2022/09/02 2022年度初版
*/
#pragma once


#include "../Renderer.h"

class ForwardRenderer : public Renderer
{
	IN_DEVELOPMENT("別スレッド上での動作を開発予定 & LightMap 生成待ち")
	SUB_CLASS(ForwardRenderer)
public:

	bool Initialize() override;
	void Shutdown() override;

	void Update() noexcept;
	void Present() noexcept;

private:

	// * Render と Present
	Array<Job, 2> m_jobs;
};