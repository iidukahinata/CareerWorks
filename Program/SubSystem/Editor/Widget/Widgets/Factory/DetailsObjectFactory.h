/**
* @file	   DetailsObjectFactory.h
* @brief
*
* @date	   2022/09/13 2022�N�x����
*/
#pragma once


#include "../DetailsObject/DetailsObject.h"

class GameObject;
class ResourceData;

/**
* �R���|�[�l���g�̐������V�X�e�����番�������邽�߂ɍ쐬�B
* �V�����R���|�[�l���g���쐬�����ꍇ�A���L�֐����ɒǉ����Ă����B
*/
struct DetailsObjectFactory
{
	static Vector<UniquePtr<DetailsObject>> Create(DetailsWidget* detailsWidget, GameObject* gameObject) noexcept;
	static Vector<UniquePtr<DetailsObject>> Create(DetailsWidget* detailsWidget, ResourceData* resourceData) noexcept;
};