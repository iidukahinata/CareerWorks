/**
* @file	   AudioListenerDetails.h
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class IAudioListener;

class AudioListenerDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	AudioListenerDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	IAudioListener* m_audioListener = nullptr;
};