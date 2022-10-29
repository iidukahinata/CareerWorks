/**
* @file	   PostProcessEffectDetails.cpp
* @brief
*
* @date	   2022/10/29 2022年度初版
*/


#include "PostProcessEffectDetails.h"
#include "SubSystem/Scene/Component/Components/PostProcessEffect.h"

#include "SubSystem/Renderer/PostEffect/Blur.h"
#include "SubSystem/Renderer/PostEffect/Bloom.h"
#include "SubSystem/Renderer/PostEffect/Monotone.h"
#include "SubSystem/Renderer/PostEffect/DepthOfField.h"

PostProcessEffectDetails::PostProcessEffectDetails(DetailsWidget* detailsWidget, IComponent* component) : ComponentDetails(detailsWidget)
{
	m_postProcessEffect = dynamic_cast<PostProcessEffect*>(component);
	ASSERT(m_postProcessEffect);

	// Add PostEffect 時の検索に使用
	RegisterPostEffect<Blur>();
	RegisterPostEffect<Bloom>();
	RegisterPostEffect<Monotone>();
	RegisterPostEffect<DepthOfField>();
}

void PostProcessEffectDetails::Draw()
{
	if (ShowComponentHeader(m_postProcessEffect))
	{
		constexpr int offsetPos = 130;
		auto width = ImGui::GetWindowWidth() - offsetPos;

		ImGui::Text(""); ImGui::SameLine(65);
		if (ImGui::Button("Add PostEffect", ImVec2(width, 20)))
		{
			ImGui::OpenPopup("AddPostEffect");
		}

		ShowAddPostEffectWindow();
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