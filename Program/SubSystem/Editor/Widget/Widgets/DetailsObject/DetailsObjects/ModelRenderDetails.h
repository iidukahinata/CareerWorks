/**
* @file	   ModelRenderDetails.h
* @brief
*
* @date	   2022/09/13 2022�N�x����
*/
#pragma once


#include "../DetailsObject.h"

class Model;
class IComponent;
class ModelRender;

class ModelRenderDetails : public DetailsObject
{
public:

	ModelRenderDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	/** ���f���ɓK�p���̃��b�V���ꗗ�\�� */
	void ShowUseMeshes(Model* model) const noexcept;

	/** ���f���ɓK�p���̃}�e���A���ꗗ�\�� */
	void ShowUseMaterial(Model* model) noexcept;

private:

	ModelRender* m_modelRender;
};