/**
* @file    ModuleGenerator.cpp
* @brief
*
* @date	   2022/11/15 2022îNìxèâî≈
*/


#include "ModuleHelper.h"

BOOST_PYTHON_MODULE(Engine)
{
	SetUpCoreModule();
	SetUpMathModule();
	SetUpInputModule();
	SetUpResourceModule();
	SetUpSceneModule();
	SetUpComponentModule();
}

/**
BOOST_PYTHON_MODULE(Core)
{
	SetUpCoreModule();
	SetUpSceneModule();
}

BOOST_PYTHON_MODULE(Math)
{
	SetUpMathModule();
}

BOOST_PYTHON_MODULE(Input)
{
	SetUpInputModule();
}

BOOST_PYTHON_MODULE(Resource)
{
	SetUpResourceModule();
}

BOOST_PYTHON_MODULE(Component)
{
	SetUpComponentModule();
}
*/