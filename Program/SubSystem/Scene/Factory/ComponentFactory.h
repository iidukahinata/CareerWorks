/**
* @file    ComponentFactory.h
* @brief
*
* @date	   2022/08/02 2022年度初版
*/
#pragma once


#include "../Component/IComponent.h"

/**
* コンポーネントの生成をシステムから分離させるために作成。
* 新しいコンポーネントを作成した場合、下記関数内に追加していく。
*/
struct ComponentFactory
{
	static UniquePtr<IComponent> Create(GameObject* gameObject, StringView name) noexcept;
};