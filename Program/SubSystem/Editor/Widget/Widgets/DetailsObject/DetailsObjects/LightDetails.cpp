/**
* @file	   LightDetails.cpp
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/


#include "LightDetails.h"
#include "SubSystem/Scene/Component/Components/Light.h"

LightDetails::LightDetails(DetailsWidget* detailsWidget, IComponent* component) :
	DetailsObject(detailsWidget)
{
	m_light = dynamic_cast<Light*>(component);
	ASSERT(m_light);
}

void LightDetails::Draw()
{
	constexpr auto offsetPos = 130;
	constexpr auto lightTypeCombo = "DirectionalLight\0PointLight\0SpotLight\0\0";
		
	const auto& name = m_light->TypeData.Name;
	if (ImGui::CollapsingHeader(name.data(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto lightType		= m_light->GetLightType();
		auto color			= m_light->GetColor();
		auto intensity		= m_light->GetIntensity();
		auto influenceRange = m_light->GetInfluenceRange();
		auto angle			= m_light->GetAngle();

		ImGui::Text("LightType"); ImGui::SameLine(offsetPos);
		auto inputLightType = ImGui::Combo("##LightType", (int*)(&lightType), lightTypeCombo);

		ImGui::Text("Color"); ImGui::SameLine(offsetPos);
		auto inputColor = ImGui::ColorEdit4("##Color", &color.x);

		ImGui::Text("Intensity"); ImGui::SameLine(offsetPos);
		auto inputIntensity = ImGui::DragFloat("##Intensity", &intensity, 0.f);

		auto inputIntensityRange = false;
		if (lightType == LightType::PointLight || lightType == LightType::SpotLight)
		{
			ImGui::Text("InfluenceRange"); ImGui::SameLine(offsetPos);
			inputIntensityRange = ImGui::DragFloat("##InfluenceRange", &influenceRange, 0.f);
		}

		auto inputAngle = false;
		if (lightType == LightType::SpotLight)
		{
			ImGui::Text("Angle"); ImGui::SameLine(offsetPos);
			inputAngle = ImGui::DragFloat("##Angle", &angle);
		}

		if (inputLightType)		 m_light->SetLightType(lightType);
		if (inputColor)			 m_light->SetColor(color);
		if (inputIntensity)		 m_light->SetIntensity(intensity);
		if (inputIntensityRange) m_light->SetInfluenceRange(influenceRange);
		if (inputAngle)			 m_light->SetAngle(angle);
	}
}