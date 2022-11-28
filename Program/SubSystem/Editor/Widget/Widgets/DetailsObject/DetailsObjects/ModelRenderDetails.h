/**
* @file	   ModelRenderDetails.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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

	/** モデルに適用中のメッシュ一覧表示 */
	void ShowUseMeshes(Model* model) const noexcept;

	/** モデルに適用中のマテリアル一覧表示 */
	void ShowUseMaterial(Model* model) noexcept;

private:

	GameObject* m_gameObject = nullptr;

	IModelRender* m_modelRender = nullptr;
};