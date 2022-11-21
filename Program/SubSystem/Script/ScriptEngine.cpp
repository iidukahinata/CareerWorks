/**
* @file    ScriptEngine.cpp
* @brief
*
* @date	   2022/11/13 2022îNìxèâî≈
*/


#include "ScriptEngine.h"
#include "Module/ModuleHelper.h"

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

bool ScriptEngine::Initialize()
{
	InitModule();
	Py_Initialize();
	return true;
}