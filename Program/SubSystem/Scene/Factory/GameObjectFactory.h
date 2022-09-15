/**
* @file    GameObjectFactory.h
* @brief
*
* @date	   2022/07/06 2022年度初版
*/
#pragma once


#include "../GameObject.h"

/**
* ゲームオブジェクトの生成をシステムから分離させるために作成。
*/
struct GameObjectFactory
{
	static UniquePtr<GameObject> Create(World* world, Scene* scene);
};