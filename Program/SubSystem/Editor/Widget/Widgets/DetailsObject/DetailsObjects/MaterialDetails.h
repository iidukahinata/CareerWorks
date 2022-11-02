/**
* @file	   MaterialDetails.h
* @brief
*
* @date	   2022/10/27 2022年度初版
*/
#pragma once


#include "../DetailsObject.h"

class Texture;
class Material;
class ResourceData;

class MaterialDetails : public DetailsObject
{
	WAIT_FOR_DEVELOPMENT("Scene内でPBR用シェーダーに変更した際に ImGUi Descriptor エラーが発生する")
public:

	MaterialDetails(DetailsWidget* detailsWidget, ResourceData* resourceData);
	~MaterialDetails();

	void Draw() override;

private:

	/** 実際の Interface 提供 */
	void ShowMaterialInterface() noexcept;

	/** 使用中シェーダー一覧表示 */
	void ShowShaderList(Material* material) noexcept;

	/** 使用中テクスチャ一覧表示 */
	void ShowTextureList(Material* material) noexcept;

	/** テクスチャのドラッグアンドドロップ対応 */
	bool DragDropTexture(Material* material, Texture* texture, StringView pramName) noexcept;

	/** 開かれているシーン内リソースかを判別 */
	bool IsCurrentSceneResource() const noexcept;

private:

	ResourceHandle* m_resourceHandle = nullptr;
	Material* m_material			 = nullptr;

	bool m_useGS = false;
};