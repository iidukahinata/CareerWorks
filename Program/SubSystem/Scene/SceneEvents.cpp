/**
* @file    SceneEvents.h
* @brief
*
* @date	   2022/09/05 2022”N“x‰”Å
*/


#include "SceneEvents.h"

ChangeSceneEvent::ChangeSceneEvent(Scene* scene) :
	m_scene(scene)
{

}

std::any ChangeSceneEvent::GetData()
{
	return m_scene;
}

LoadSceneCompleteEvent::LoadSceneCompleteEvent(const String& name)
	: m_name(name)
{

}

std::any LoadSceneCompleteEvent::GetData()
{
	return m_name;
}