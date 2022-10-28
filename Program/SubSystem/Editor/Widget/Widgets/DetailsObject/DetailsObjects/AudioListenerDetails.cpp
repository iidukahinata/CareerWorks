/**
* @file	   AudioListenerDetails.cpp
* @brief
*
* @date	   2022/10/27 2022”N“x‰”Å
*/

#include "AudioListenerDetails.h"
#include "SubSystem/Scene/Component/Components/AudioListener.h"

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