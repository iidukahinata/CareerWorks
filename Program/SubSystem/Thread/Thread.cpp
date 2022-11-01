/**
* @file    Thread.cpp
* @brief
*
* @date	   2022/11/01 2022”N“x‰”Å
*/


#include "Thread.h"

void IThread::RequestQuit() noexcept
{
	m_isRequestQuit.store(true);
}

bool IThread::IsRequestQuit() const noexcept
{
	return m_isRequestQuit.load();
}