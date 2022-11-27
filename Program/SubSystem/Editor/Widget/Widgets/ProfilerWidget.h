/**
* @file	   ProfilerWidget.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class ProfilerWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void Draw() override;

private:

	void ShowTimeLine() noexcept;
};