/**
* @file	   ScriptDetails.h
* @brief
*
* @date	   2022/11/16 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class Script;

class ScriptDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	ScriptDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	Script* m_script = nullptr;
};