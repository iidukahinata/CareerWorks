/**
* @file	   AudioListenerDetails.h
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class AudioListener;
class IComponent;

class AudioListenerDetails : public DetailsObject
{
public:

	AudioListenerDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	AudioListener* m_audioListener;
};