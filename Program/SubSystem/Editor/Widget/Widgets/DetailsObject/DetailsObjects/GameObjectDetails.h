/**
* @file	   GameObjectDetails.h
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class GameObject;

class GameObjectDetails : public DetailsObject
{
public:

	GameObjectDetails(DetailsWidget* detailsWidget, GameObject* gameObject);
	void Draw() override;

private:

	GameObject* m_gameObject;
};