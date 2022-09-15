/**
* @file	   GameObjectDetails.cpp
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
*/


#include "GameObjectDetails.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"

GameObjectDetails::GameObjectDetails(DetailsWidget* detailsWidget, GameObject* gameObject) :
	DetailsObject(detailsWidget), m_gameObject(gameObject)
{

}

void GameObjectDetails::Draw()
{

}