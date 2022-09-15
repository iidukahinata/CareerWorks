/**
* @file    ResourceFactory.h
* @brief
*
* @date	   2022/08/08 2022年度初版
*/
#pragma once


class IResource;
class ResourceManager;

/**
* リソース生成をシステムから分離させるために作成。
* 新しいリソースタイプを作成した場合、下記関数内に追加していく。
*/
struct ResourceFactory
{
	static UniquePtr<IResource> Create(uint32_t type, StringView path, ResourceManager* resourceManager);
};