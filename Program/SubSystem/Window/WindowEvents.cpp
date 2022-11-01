/**
* @file    WindowEvents.cpp
* @brief
*
* @date	   2022/11/01 2022”N“x‰”Å
*/


#include "WindowEvents.h"

DestroyWindowEvent::DestroyWindowEvent()
{

}

std::any DestroyWindowEvent::GetData()
{
	return std::any();
}

CommandWindowEvent::CommandWindowEvent(int cmd) :
	m_cmd(cmd)
{

}

std::any CommandWindowEvent::GetData()
{
	return m_cmd;
}