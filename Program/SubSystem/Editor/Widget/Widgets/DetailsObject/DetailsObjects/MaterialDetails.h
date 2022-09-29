/**
* @file	   MaterialDetails.h
* @brief
*
* @date	   2022/09/13 2022年度初版
*/
#pragma once


#include "../DetailsObject.h"

class Material;
class ResourceData;

class MaterialDetails : public DetailsObject
{
public:

	MaterialDetails(DetailsWidget* detailsWidget, ResourceData* resourceData);
	void Draw() override;

private:

	/** 実際の Interface 提供 */
	void ShowMaterialInterface() noexcept;

	/** 使用中シェーダー一覧表示 */
	void ShowShaderList(Material* material) noexcept;

	/** 使用中テクスチャ一覧表示 */
	void ShowTextureList(Material* material) noexcept;

private:

	ResourceHandle* m_resourceHandle;
	Material* m_material;
};