/**
* @file    ResourceFactory.h
* @brief
*
* @date	   2022/08/08 2022�N�x����
*/
#pragma once


class IResource;
class ResourceManager;

/**
* ���\�[�X�������V�X�e�����番�������邽�߂ɍ쐬�B
* �V�������\�[�X�^�C�v���쐬�����ꍇ�A���L�֐����ɒǉ����Ă����B
*/
struct ResourceFactory
{
	static UniquePtr<IResource> Create(uint32_t type, StringView path, ResourceManager* resourceManager);
};