/**
* @file    SceneEvents.h
* @brief
*
* @date	   2022/09/05 2022�N�x����
*/
#pragma once


class Scene;

class ChangeSceneEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ChangeSceneEvent)
public:

	ChangeSceneEvent(Scene* scene);
	std::any GetData() override;

private:

	// * �ύX��V�[���I�u�W�F�N�g
	Scene* m_scene;
};

class LoadSceneCompleteEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(LoadSceneCompleteEvent)
public:

	LoadSceneCompleteEvent(const String& name);
	std::any GetData() override;

private:

	// * �ǂݍ��񂾃V�[����
	String m_name;
};