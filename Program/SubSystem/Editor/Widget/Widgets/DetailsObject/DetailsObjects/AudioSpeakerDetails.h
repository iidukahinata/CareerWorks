/**
* @file	   AudioSpeakerDetails.h
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class AudioSpeaker;
class IComponent;

class AudioSpeakerDetails : public DetailsObject
{
public:

	AudioSpeakerDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	AudioSpeaker* m_audioSpeaker;
};