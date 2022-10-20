/**
* @file	   AudioListenerDetails.cpp
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/

#include "AudioListenerDetails.h"
#include "SubSystem/Scene/Component/Components/AudioListener.h"

AudioListenerDetails::AudioListenerDetails(DetailsWidget* detailsWidget, IComponent* component) :
	DetailsObject(detailsWidget)
{
	m_audioListener = dynamic_cast<AudioListener*>(component);
	ASSERT(m_audioListener);
}

void AudioListenerDetails::Draw()
{
	constexpr int offsetPos = 130;
	const auto& name = m_audioListener->TypeData.Name;

	ImGui::CollapsingHeader(name.data(), ImGuiTreeNodeFlags_DefaultOpen);
}