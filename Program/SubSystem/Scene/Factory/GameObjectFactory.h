/**
* @file    GameObjectFactory.h
* @brief
*
* @date	   2022/07/06 2022�N�x����
*/
#pragma once


#include "../GameObject.h"

/**
* �Q�[���I�u�W�F�N�g�̐������V�X�e�����番�������邽�߂ɍ쐬�B
*/
struct GameObjectFactory
{
	static UniquePtr<GameObject> Create(World* world, Scene* scene);
};