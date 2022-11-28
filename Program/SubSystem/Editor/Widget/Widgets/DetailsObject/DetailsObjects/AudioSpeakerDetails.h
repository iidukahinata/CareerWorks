/**
* @file	   AudioSpeakerDetails.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class IAudioSpeaker;

class AudioSpeakerDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	AudioSpeakerDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	IAudioSpeaker* m_audioSpeaker = nullptr;
};