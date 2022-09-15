/**
* @file	   ModelRenderDetails.h
* @brief
*
* @date	   2022/09/13 2022年度初版
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

	/** モデルに適用中のメッシュ一覧表示 */
	void ShowUseMeshes(Model* model) const noexcept;

	/** モデルに適用中のマテリアル一覧表示 */
	void ShowUseMaterial(Model* model) noexcept;

private:

	ModelRender* m_modelRender;
};