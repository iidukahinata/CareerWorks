/**
* @file	   MaterialDetails.h
* @brief
*
* @date	   2022/09/13 2022年度初版
*/
#pragma once


#include "../DetailsObject.h"

class Texture;
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

private:

	ResourceHandle* m_resourceHandle;
	Material* m_material;
};