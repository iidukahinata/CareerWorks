/**
* @file	   MaterialDetails.cpp
* @brief
*
* @date	   2022/10/27 2022年度初版
*/


#include "MaterialDetails.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

MaterialDetails::MaterialDetails(DetailsWidget* detailsWidget, ResourceData* resourceData) :
	DetailsObject(detailsWidget)
{
	if (auto resource = m_detailsWidget->GetResourceManager()->GetResource(resourceData))
	{
		m_material = dynamic_cast<Material*>(resource);
	}
	else
	{
		m_resourceHandle = m_detailsWidget->GetResourceManager()->Load(resourceData);
	}
}

MaterialDetails::~MaterialDetails()
{
	if (!m_material)
	{
		return;
	}

	m_material->Update();

	if (!IsCurrentSceneResource())
	{
		auto resourceManager = m_detailsWidget->GetResourceManager();
		if (auto resourceData = resourceManager->GetResourceData(m_material->GetFilePath()))
		{
			resourceManager->Unload(resourceData);
		}
	}
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
		auto material	= m_material;
		auto path		= ConvertToJapanese(material->GetFilePath());
		auto blendMode	= material->GetBlendMode();
		auto rasterizer	= material->GetRasterizerState();
		auto albedo		= material->GetAlbedo();
		auto metallic	= material->GetMetallic();
		auto smooth		= material->GetSmooth();
		auto emissive	= material->GetEmission();

		ImGui::Text("Name"); ImGui::SameLine(offsetPos);
		ImGui::Text(path.c_str());

		ShowShaderList(material);

		ImGui::Text("BlendMode"); ImGui::SameLine(offsetPos);
		auto inputBlendMode = ImGui::Combo("##BlendMode", (int*)(&blendMode), blendModeCombo);

		ImGui::Text("Rasterizer"); ImGui::SameLine(offsetPos);
		auto inputRasterizer = ImGui::Combo("##Rasterizer", (int*)(&rasterizer), rasterizerCombo);

		ImGui::Text("Albedo"); ImGui::SameLine(offsetPos);
		auto inputAlbedo = ImGui::DragFloat3("##Albedo", &albedo.x, 0.1f, 0.f);

		ImGui::Text("Metallic"); ImGui::SameLine(offsetPos);
		auto inputMetallic = ImGui::SliderFloat("##Metallic", &metallic, 0.f, 1.f);

		ImGui::Text("Smooth"); ImGui::SameLine(offsetPos);
		auto inputSmooth = ImGui::SliderFloat("##Smooth", &smooth, 0.f, 1.f);

		ImGui::Text("Emissive"); ImGui::SameLine(offsetPos);
		auto inputEmissive = ImGui::DragFloat3("##Emissive", &emissive.x, 0.1f, 0.f);

		if (ImGui::TreeNodeEx("Texture Prametor", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ShowTextureList(material);
		}

		if (inputBlendMode)  RegisterEditorCommand([material](auto data) { material->SetBlendMode(data);	   }, blendMode	 , material->GetBlendMode());
		if (inputRasterizer) RegisterEditorCommand([material](auto data) { material->SetRasterizerState(data); }, rasterizer , material->GetRasterizerState());
		if (inputAlbedo)     RegisterEditorCommand([material](auto data) { material->SetAlbedo(data);		   }, albedo	 , material->GetAlbedo());
		if (inputMetallic)   RegisterEditorCommand([material](auto data) { material->SetMetallic(data);		   }, metallic	 , material->GetMetallic());
		if (inputSmooth)	 RegisterEditorCommand([material](auto data) { material->SetSmooth(data);		   }, smooth	 , material->GetSmooth());
		if (inputEmissive)   RegisterEditorCommand([material](auto data) { material->SetEmission(data);		   }, emissive	 , material->GetEmission());
	}
}

void MaterialDetails::ShowShaderList(Material* material) noexcept
{
	constexpr auto offsetPos = 130;

	const auto& shaderPaths = material->GetShaderPaths();
	auto shaderPath = shaderPaths[VertexShader];

	ImGui::Text("Shader"); ImGui::SameLine(offsetPos);

	constexpr auto itemWidth = 250;
	ImGui::PushItemWidth(itemWidth);
	ImGui::InputText("##", shaderPath.data(), shaderPath.size());
	ImGui::PopItemWidth();

	ImGui::SameLine(offsetPos + itemWidth + 5);
	
	OpenResourceHelper("Use Shader");
	if (auto resourceData = ShowSearchResourceHelper<Shader>())
	{
		auto useGS = m_useGS;
		const auto& nextShaderPath = resourceData->m_resourcePath.m_path;

		RegisterEditorCommand([material, useGS](auto data) {

				material->SetShader(VertexShader, data, false);

				if (useGS)
				{
					material->SetShader(PixelShader, data, false);
					material->SetShader(GeometryShader, data);
				}
				else
				{
					material->SetShader(PixelShader, data);
				}
			}
			, nextShaderPath, shaderPath
		);
	}

	ImGui::Text("");

	ImGui::Text("Use GS Stage"); ImGui::SameLine(offsetPos);
	auto inputUse = ImGui::RadioButton("##Use GeometryShader", m_useGS);

	if (inputUse)
	{
		m_useGS = !m_useGS;
	}
}

void MaterialDetails::ShowTextureList(Material* material) noexcept
{
	constexpr auto offsetPos = 130;

	UINT texIndex = 0;
	for (const auto& textureInfo : material->GetTextures())
	{
		const auto& pramName = textureInfo.first;
		const auto& texture  = textureInfo.second.m_texture;

		auto texturePath = texture ? ConvertToJapanese(texture->GetFilePath().c_str()) : String();

		// テクスチャ名表示
		ImGui::Text(pramName.c_str()); ImGui::SameLine(offsetPos);

		constexpr auto itemWidth = 250;
		ImGui::PushItemWidth(itemWidth);
		ImGui::InputText("##", texturePath.data(), texturePath.size());
		ImGui::PopItemWidth();

		// Texture 未設定時は文字列表示部分でドラッグアンドドロップを行う
		if (!texture)
		{
			DragDropTexture(material, texture, pramName);
		}

		ImGui::SameLine(offsetPos + itemWidth + 5);

		// 検索での Texture 切り替え
		OpenResourceHelper(pramName);
		if (auto resourceData = ShowSearchResourceHelper<Texture>())
		{
			if (auto catchTexture = LoadResource<Texture>(resourceData))
			{
				RegisterEditorCommand([material, pramName](auto data) { material->SetTexture(pramName, data); }, catchTexture, texture);
			}
		}

		ImGui::Text("");

		if(!texture)
		{
			continue;
		}

		EditorHelper::Get().AddImage(texture->GetData(), ImVec2(100, 100));
		DragDropTexture(material, texture, pramName);
		ImGui::Text("");
	}

	ImGui::TreePop();
}

bool MaterialDetails::DragDropTexture(Material* material, Texture* texture, StringView pramName) noexcept
{
	auto result = false;

	// ドラッグアンドドロップでの Texture 切り替え
	const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
	if (ImGui::IsMouseReleased(0) && hoverd)
	{
		if (auto catchTexture = CatchDragObject<Texture>())
		{
			RegisterEditorCommand([material, pramName](auto data) { material->SetTexture(pramName, data); }, catchTexture, texture);
			result = true;
		}
	}

	ShowDragDropHelper<Texture>(hoverd, 156, 0, 31);

	return result;
}

bool MaterialDetails::IsCurrentSceneResource() const noexcept
{
	return m_resourceHandle == nullptr;
}