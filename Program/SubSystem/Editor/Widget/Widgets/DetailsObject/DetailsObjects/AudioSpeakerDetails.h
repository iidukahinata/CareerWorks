/**
* @file	   AudioSpeakerDetails.h
* @brief
*
* @date	   2022/10/23 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class AudioSpeaker;

class AudioSpeakerDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	AudioSpeakerDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	AudioSpeaker* m_audioSpeaker;
};