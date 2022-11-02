/**
* @file	   MaterialDetails.h
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/
#pragma once


#include "../DetailsObject.h"

class Texture;
class Material;
class ResourceData;

class MaterialDetails : public DetailsObject
{
	WAIT_FOR_DEVELOPMENT("Scene����PBR�p�V�F�[�_�[�ɕύX�����ۂ� ImGUi Descriptor �G���[����������")
public:

	MaterialDetails(DetailsWidget* detailsWidget, ResourceData* resourceData);
	~MaterialDetails();

	void Draw() override;

private:

	/** ���ۂ� Interface �� */
	void ShowMaterialInterface() noexcept;

	/** �g�p���V�F�[�_�[�ꗗ�\�� */
	void ShowShaderList(Material* material) noexcept;

	/** �g�p���e�N�X�`���ꗗ�\�� */
	void ShowTextureList(Material* material) noexcept;

	/** �e�N�X�`���̃h���b�O�A���h�h���b�v�Ή� */
	bool DragDropTexture(Material* material, Texture* texture, StringView pramName) noexcept;

	/** �J����Ă���V�[�������\�[�X���𔻕� */
	bool IsCurrentSceneResource() const noexcept;

private:

	ResourceHandle* m_resourceHandle = nullptr;
	Material* m_material			 = nullptr;

	bool m_useGS = false;
};