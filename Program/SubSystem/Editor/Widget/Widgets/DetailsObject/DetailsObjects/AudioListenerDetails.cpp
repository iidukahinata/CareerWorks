/**
* @file	   AudioListenerDetails.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/

#include "AudioListenerDetails.h"
#include "SubSystem/Scene/Component/IAudioListener.h"

AudioListenerDetails::AudioListenerDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_audioListener = dynamic_cast<IAudioListener*>(component);
	ASSERT(m_audioListener);
}

void AudioListenerDetails::Draw()
{
	ShowComponentHeader(m_audioListener);
}