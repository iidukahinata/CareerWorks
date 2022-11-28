/**
* @file	   ModelRenderDetails.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../ComponentDetails.h"

class Model;
class IModelRender;

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

	GameObject* m_gameObject = nullptr;

	IModelRender* m_modelRender = nullptr;
};