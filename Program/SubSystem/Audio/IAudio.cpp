/**
* @file    Audio.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "IAudio.h"

void IAudio::SetAudioListener(AudioListener* listener) noexcept
{
	m_lisrener = listener;
}