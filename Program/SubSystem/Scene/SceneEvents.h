/**
* @file    SceneEvents.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


class Scene;

class ChangeSceneEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ChangeSceneEvent, IEvent)
public:

	ChangeSceneEvent(Scene* scene);
	std::any GetData() override;

private:

	// * 変更先シーンオブジェクト
	Scene* m_scene;
};

class LoadSceneCompleteEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(LoadSceneCompleteEvent, IEvent)
public:

	LoadSceneCompleteEvent(const String& name);
	std::any GetData() override;

private:

	// * 読み込んだシーン名
	String m_name;
};