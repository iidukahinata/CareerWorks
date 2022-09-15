/**
* @file	   MaterialDetails.cpp
* @brief
*
* @date	   2022/09/13 2022年度初版
*/


#include "MaterialDetails.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

MaterialDetails::MaterialDetails(DetailsWidget* detailsWidget, ResourceData* resourceData) :
	DetailsObject(detailsWidget)
{
	m_resourceHandle = m_detailsWidget->GetResourceManager()->Load(resourceData);
}

void MaterialDetails::Draw()
{
	if (!m_material)
	{
		// Load 完了したタイミングでリソースを受け取る
		if (m_resourceHandle && m_resourceHandle->IsValid())
		{
			m_material = m_resourceHandle->GetResource<Material>();
		}
	}
	else
	{
		ShowMaterialInterface();
	}
}

void MaterialDetails::ShowMaterialInterface() noexcept
{
	constexpr auto offsetPos = 130;
	constexpr auto blendModeCombo = "BLEND_MODE_NO_ALPHA\0BLEND_MODE_ALPHA\0BLEND_MODE_ADD\0BLEND_MODE_ADD_ALPHA\0BLEND_MODE_SUB\0BLEND_MODE_SCREEN\0\0";
	constexpr auto rasterizerCombo = "NO_CULL\0CULL_CLOCK_WISE\0WIRE_FRAME_CULL\0DEPTH_TILT_BIAS\0\0";

	if (ImGui::CollapsingHeader("Material Interface", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto path			= ConvertToJapanese(m_material->GetFilePath());
		auto textures		= m_material->GetTextures();
		auto blendMode		= m_material->GetBlendMode();
		auto rasterizer		= m_material->GetRasterizerState();
		auto ambient		= m_material->GetAmbient();
		auto diffuse		= m_material->GetDiffuse();
		auto emissive		= m_material->GetEmissive();
		auto specular		= m_material->GetSpecular();
		auto specularPower	= m_material->GetSpecularPower();

		ImGui::Text("Name"); ImGui::SameLine(offsetPos);
		ImGui::Text(path.c_str()); ImGui::Text("");

		ImGui::Text("BlendMode"); ImGui::SameLine(offsetPos);
		auto inputBlendMode = ImGui::Combo("##BlendMode", (int*)(&blendMode), blendModeCombo);

		ImGui::Text("Rasterizer"); ImGui::SameLine(offsetPos);
		auto inputRasterizer = ImGui::Combo("##Rasterizer", (int*)(&rasterizer), rasterizerCombo);

		ImGui::Text("Ambient"); ImGui::SameLine(offsetPos);
		auto inputAmbient = ImGui::DragFloat3("##Ambient", &ambient.x, 0.f, 1.f);

		ImGui::Text("Diffuse"); ImGui::SameLine(offsetPos);
		auto inputDiffuse = ImGui::DragFloat3("##Diffuse", &diffuse.x, 0.f, 1.f);

		ImGui::Text("Emissive"); ImGui::SameLine(offsetPos);
		auto inputEmissive = ImGui::DragFloat3("##Emissive", &emissive.x, 0.f, 1.f);

		ImGui::Text("Specular"); ImGui::SameLine(offsetPos);
		auto inputSpecular = ImGui::DragFloat3("##Specular", &specular.x, 0.f, 1.f);

		ImGui::Text("SpecularPower"); ImGui::SameLine(offsetPos);
		auto inputSprPower = ImGui::DragFloat("##SpecularPower", &specularPower, 0.f, 1.f);

		if (ImGui::TreeNodeEx("Texture Prametor", ImGuiTreeNodeFlags_DefaultOpen))
		{
			int index = 0;
			for (const auto& textureInfo : textures)
			{
				const auto& pramName = textureInfo.first;
				const auto& texture = textureInfo.second.m_texture;

				auto texPath = texture ? ConvertToJapanese(texture->GetFilePath().c_str()) : nullptr;

				ImGui::Text(pramName.c_str()); ImGui::SameLine(offsetPos);
				ImGui::Text(texPath.c_str());

				// ドラッグアンドドロップ有効指定
				const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

				// Texture 受付用 UI 表示
				ShowDragDropHelper<Texture>(hoverd, 220, 130, 0);

				// ドラッグアンドドロップでの Texture 切り替えのため
				if (ImGui::IsMouseReleased(0) && hoverd)
				{
					if (auto catchTexture = CatchDragObject<Texture>())
					{
						m_material->SetTexture(pramName, catchTexture);
					}
				}

				// 改行用
				ImGui::Text("");
			}
			ImGui::TreePop();
		}

		if (inputBlendMode)  m_material->SetBlendMode(blendMode);
		if (inputRasterizer) m_material->SetRasterizerState(rasterizer);
		if (inputAmbient)    m_material->SetAmbient(ambient);
		if (inputDiffuse)    m_material->SetDiffuse(diffuse);
		if (inputEmissive)   m_material->SetEmissive(emissive);
		if (inputSpecular)   m_material->SetSpecular(specular);
		if (inputSprPower)   m_material->SetSpecularPower(specularPower);
	}
}