/**
* @file    IAudio.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "IAudio.h"

void IAudio::SetAudioListener(IAudioListener* listener) noexcept
{
	m_lisrener = listener;
}