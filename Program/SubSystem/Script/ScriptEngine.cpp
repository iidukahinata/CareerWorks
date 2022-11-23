/**
* @file    ScriptEngine.cpp
* @brief
*
* @date	   2022/11/23 2022îNìxèâî≈
*/


#include "ScriptEngine.h"
#include "Module/ModuleHelper.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Script/ScriptInstance.h"

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

bool ScriptEngine::Initialize()
{
	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	InitModule();
	Py_Initialize();
	return true;
}

void ScriptEngine::RebuildAllScript() noexcept
{
	try
	{
		boost::python::object importlib = boost::python::import("importlib").attr("__dict__");
		if (!importlib.contains("reload")) {
			LOG_ERROR("Not Find reload() from importlib");
			return;
		}

		auto resources = m_resourceManager->GetResourcesByType<ScriptInstance>();
		for (const auto& resource : resources)
		{
			auto script = dynamic_cast<ScriptInstance*>(resource);
			importlib["reload"](boost::python::import(script->GetScriptName().c_str()));
			script->Load(script->GetFilePath());
		}
	}
	catch (const std::exception&)
	{
		LOG_ERROR("Script Refresh Error");
	}
}