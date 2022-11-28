/**
* @file	   PostProcessEffectDetails.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "PostProcessEffectDetails.h"
#include "SubSystem/Scene/Component/IPostProcessEffect.h"

#include "SubSystem/Renderer/PostEffect/Blur.h"
#include "SubSystem/Renderer/PostEffect/Bloom.h"
#include "SubSystem/Renderer/PostEffect/DepthOfField.h"

PostProcessEffectDetails::PostProcessEffectDetails(DetailsWidget* detailsWidget, IComponent* component) : ComponentDetails(detailsWidget)
{
	m_postProcessEffect = dynamic_cast<IPostProcessEffect*>(component);
	ASSERT(m_postProcessEffect);

	// Add PostEffect 時の検索に使用
	RegisterPostEffect<Bloom>();
	//RegisterPostEffect<DepthOfField>();
}

void PostProcessEffectDetails::Draw()
{
	if (ShowComponentHeader(m_postProcessEffect))
	{
		constexpr int offsetPos = 130;
		auto width = ImGui::GetWindowWidth() - offsetPos;

		ShowAllPostEffect();

		ImGui::Text(""); ImGui::SameLine(65);
		if (ImGui::Button("Add PostEffect", ImVec2(width, 20)))
		{
			ImGui::OpenPopup("AddPostEffect");
		}

		ShowAddPostEffectWindow();
	}
}

void PostProcessEffectDetails::ShowAllPostEffect() noexcept
{
	for (auto& postEffect : m_postProcessEffect->GetAllPostEffect())
	{
		auto effectHash = postEffect.second->GetTypeData().Hash;
		auto effectName = postEffect.second->GetTypeData().Name;

		const ImGuiTreeNodeFlags treeNodeFlags =
			ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (ImGui::CollapsingHeader(effectName.data(), treeNodeFlags))
		{
			switch (effectHash)
			{
			case GET_HASH(Bloom): ShowBloom(postEffect.second.get());
			default: break;
			}
		}
	}
}

void PostProcessEffectDetails::ShowAddPostEffectWindow() noexcept
{
	if (ImGui::BeginPopup("AddPostEffect"))
	{
		ImGui::Text("PostEffect Name");
		m_filter.Draw("Filter", 150.0f);

		ImGui::BeginChild("##PostEffects", ImVec2(180, 250));

		for (const auto& postEffect : m_postEffects)
		{
			auto name = postEffect->GetTypeData().Name;

			if (!m_filter.PassFilter(name.data()))
				continue;

			if (m_postProcessEffect->FindPostEffect(name))
				continue;

			if (ImGui::Button(name.data(), ImVec2(180, 20)))
			{
				m_postProcessEffect->AddPostEffect(name);
			}
		}

		ImGui::EndChild();
		ImGui::EndPopup();
	}
}

void PostProcessEffectDetails::ShowBloom(IPostEffect* postEffect) noexcept
{
	constexpr auto offsetPos = 130;

	auto bloom = dynamic_cast<Bloom*>(postEffect);
	ASSERT(bloom);

	auto color = bloom->GetColor();
	auto clamp	   = bloom->GetClamp();
	auto diffusion = bloom->GetDiffusion();
	auto threshold = bloom->GetThreshold();
	auto intensity = bloom->GetIntensity();

	ImGui::Text("Intensity"); ImGui::SameLine(offsetPos);
	auto inputIntensity = ImGui::DragFloat("##BloomIntensity", &intensity, 0.01f);

	ImGui::Text("Threshold"); ImGui::SameLine(offsetPos);
	auto inputThreshold = ImGui::DragFloat("##Threshold", &threshold, 0.01f);

	ImGui::Text("Diffusion"); ImGui::SameLine(offsetPos);
	auto inputDiffusion = ImGui::DragFloat("##Diffusion", &diffusion, 0.01f);

	ImGui::Text("Clamp"); ImGui::SameLine(offsetPos);
	auto inputClamp = ImGui::DragFloat("##Clamp", &clamp, 0.01f);

	ImGui::Text("Color"); ImGui::SameLine(offsetPos);
	auto inputColor = ImGui::ColorEdit4("##Color", &color.x);

	intensity = max(intensity, 0.f);
	threshold = max(threshold, 0.f);
	diffusion = std::clamp(diffusion, 1.f, 10.f);

	if (inputIntensity) RegisterEditorCommand([bloom](auto data) { bloom->SetIntensity(data); }, intensity, bloom->GetIntensity());
	if (inputThreshold) RegisterEditorCommand([bloom](auto data) { bloom->SetThreshold(data); }, threshold, bloom->GetThreshold());
	if (inputDiffusion) RegisterEditorCommand([bloom](auto data) { bloom->SetDiffusion(data); }, diffusion, bloom->GetDiffusion());
	if (inputClamp)		RegisterEditorCommand([bloom](auto data) { bloom->SetClamp(data);	  }, clamp	  , bloom->GetClamp());
	if (inputColor)		RegisterEditorCommand([bloom](auto data) { bloom->SetColor(data);	  }, color	  , bloom->GetColor());
}