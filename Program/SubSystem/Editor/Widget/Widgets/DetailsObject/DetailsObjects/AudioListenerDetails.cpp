/**
* @file	   AudioListenerDetails.cpp
* @brief
*
* @date	   2022/10/23 2022”N“x‰”Å
*/

#include "AudioListenerDetails.h"
#include "SubSystem/Scene/Component/Components/AudioListener.h"
#include "ThirdParty/imgui/imgui_internal.h"

AudioListenerDetails::AudioListenerDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_audioListener = dynamic_cast<AudioListener*>(component);
	ASSERT(m_audioListener);
}

void AudioListenerDetails::Draw()
{
	ShowComponentHeader(m_audioListener);
}