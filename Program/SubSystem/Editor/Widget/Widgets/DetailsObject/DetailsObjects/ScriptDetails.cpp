/**
* @file	   ScriptDetails.cpp
* @brief
*
* @date	   2022/11/16 2022îNìxèâî≈
*/

#include "ScriptDetails.h"
#include "SubSystem/Scene/Component/Components/Script.h"

ScriptDetails::ScriptDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_script = dynamic_cast<Script*>(component);
	ASSERT(m_script);
}

void ScriptDetails::Draw()
{
	if (ShowComponentHeader(m_script))
	{

	}
}