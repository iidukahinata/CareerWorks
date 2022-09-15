/**
* @file	   MaterialDetails.cpp
* @brief
*
* @date	   2022/09/13 2022�N�x����
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
		// Load ���������^�C�~���O�Ń��\�[�X���󂯎��
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
		auto material		= m_material;
		auto path			= ConvertToJapanese(material->GetFilePath());
		auto textures		= material->GetTextures();
		auto blendMode		= material->GetBlendMode();
		auto rasterizer		= material->GetRasterizerState();
		auto ambient		= material->GetAmbient();
		auto diffuse		= material->GetDiffuse();
		auto emissive		= material->GetEmissive();
		auto specular		= material->GetSpecular();
		auto specularPower	= material->GetSpecularPower();

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
			for (const auto& textureInfo : textures)
			{
				const auto& pramName = textureInfo.first;
				const auto& texture = textureInfo.second.m_texture;

				auto texPath = texture ? ConvertToJapanese(texture->GetFilePath().c_str()) : nullptr;

				ImGui::Text(pramName.c_str()); ImGui::SameLine(offsetPos);
				ImGui::Text(texPath.c_str());

				// �h���b�O�A���h�h���b�v�L���w��
				const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

				// Texture ��t�p UI �\��
				ShowDragDropHelper<Texture>(hoverd, 220, 130, 0);

				// �h���b�O�A���h�h���b�v�ł� Texture �؂�ւ��̂���
				if (ImGui::IsMouseReleased(0) && hoverd)
				{
					if (auto catchTexture = CatchDragObject<Texture>())
					{
						RegisterEditorCommand([material, pramName](auto data) { material->SetTexture(pramName, data); }, catchTexture, texture);
					}
				}

				// ���s�p
				ImGui::Text("");
			}
			ImGui::TreePop();
		}

		if (inputBlendMode)  RegisterEditorCommand([material](auto data) { material->SetBlendMode(data);	   }, blendMode	   , material->GetBlendMode());
		if (inputRasterizer) RegisterEditorCommand([material](auto data) { material->SetRasterizerState(data); }, rasterizer   , material->GetRasterizerState());
		if (inputAmbient)    RegisterEditorCommand([material](auto data) { material->SetAmbient(data);		   }, ambient	   , material->GetAmbient());
		if (inputDiffuse)    RegisterEditorCommand([material](auto data) { material->SetDiffuse(data);		   }, diffuse	   , material->GetDiffuse());
		if (inputEmissive)   RegisterEditorCommand([material](auto data) { material->SetEmissive(data);		   }, emissive	   , material->GetEmissive());
		if (inputSpecular)   RegisterEditorCommand([material](auto data) { material->SetSpecular(data);		   }, specular	   , material->GetSpecular());
		if (inputSprPower)   RegisterEditorCommand([material](auto data) { material->SetSpecularPower(data);   }, specularPower, material->GetSpecularPower());
	}
}