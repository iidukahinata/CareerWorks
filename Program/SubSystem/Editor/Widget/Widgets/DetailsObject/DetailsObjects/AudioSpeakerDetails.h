/**
* @file	   AudioSpeakerDetails.h
* @brief
*
* @date	   2022/10/27 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class AudioSpeaker;

class AudioSpeakerDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	AudioSpeakerDetails(DetailsWidget* detailsWidget, IComponent* component);
	~AudioSpeakerDetails();

	void Draw() override;

private:

	AudioSpeaker* m_audioSpeaker = nullptr;
};