/**
* @file	   AudioListenerDetails.h
* @brief
*
* @date	   2022/10/27 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class AudioListener;

class AudioListenerDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	AudioListenerDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	AudioListener* m_audioListener = nullptr;
};