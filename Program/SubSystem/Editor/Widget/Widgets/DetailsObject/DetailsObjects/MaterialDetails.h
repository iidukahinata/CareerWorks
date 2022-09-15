/**
* @file	   MaterialDetails.h
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
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

	/** ÀÛ‚Ì Interface ’ñ‹Ÿ */
	void ShowMaterialInterface() noexcept;

private:

	ResourceHandle* m_resourceHandle;
	Material* m_material;
};