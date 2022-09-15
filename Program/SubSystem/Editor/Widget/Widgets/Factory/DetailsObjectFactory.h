/**
* @file	   DetailsObjectFactory.h
* @brief
*
* @date	   2022/09/13 2022年度初版
*/
#pragma once


#include "../DetailsObject/DetailsObject.h"

class GameObject;
class ResourceData;

/**
* コンポーネントの生成をシステムから分離させるために作成。
* 新しいコンポーネントを作成した場合、下記関数内に追加していく。
*/
struct DetailsObjectFactory
{
	static Vector<UniquePtr<DetailsObject>> Create(DetailsWidget* detailsWidget, GameObject* gameObject) noexcept;
	static Vector<UniquePtr<DetailsObject>> Create(DetailsWidget* detailsWidget, ResourceData* resourceData) noexcept;
};