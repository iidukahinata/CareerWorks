/**
* @file	   ScriptDetails.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class IScript;

class ScriptDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	ScriptDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	IScript* m_script = nullptr;
};