/**
* @file	   MaterialDetails.h
* @brief
*
* @date	   2022/09/13 2022�N�x����
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

	/** ���ۂ� Interface �� */
	void ShowMaterialInterface() noexcept;

	/** �g�p���V�F�[�_�[�ꗗ�\�� */
	void ShowShaderList(Material* material) noexcept;

	/** �g�p���e�N�X�`���ꗗ�\�� */
	void ShowTextureList(Material* material) noexcept;

private:

	ResourceHandle* m_resourceHandle;
	Material* m_material;
};