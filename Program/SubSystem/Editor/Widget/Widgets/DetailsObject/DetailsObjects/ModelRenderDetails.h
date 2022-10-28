/**
* @file	   ModelRenderDetails.h
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/
#pragma once


#include "../ComponentDetails.h"

class Model;
class ModelRender;

class ModelRenderDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	ModelRenderDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	/** ���f���ɓK�p���̃��b�V���ꗗ�\�� */
	void ShowUseMeshes(Model* model) const noexcept;

	/** ���f���ɓK�p���̃}�e���A���ꗗ�\�� */
	void ShowUseMaterial(Model* model) noexcept;

private:

	ModelRender* m_modelRender = nullptr;
};