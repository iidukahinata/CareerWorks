/**
* @file    GameObjectFactory.cpp
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/


#include "GameObjectFactory.h"

UniquePtr<GameObject> GameObjectFactory::Create(World* world, Scene* scene)
{
	UniquePtr<GameObject> gameObject = std::make_unique<GameObject>();
	gameObject->m_world = world;
	gameObject->m_owner = scene;
	gameObject->m_id = 0;
	gameObject->m_path = "GameObject";
	return gameObject;
}