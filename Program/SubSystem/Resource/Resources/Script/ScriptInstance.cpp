/**
* @file    ScriptInstance.cpp
* @brief
*
* @date	   2022/11/20 2022îNìxèâî≈
*/


#include "ScriptInstance.h"
#include "SubSystem/Scene/Component/Components/Script.h"
#include <boost/python.hpp>

ScriptInstance* ScriptInstance::Create(StringView name) noexcept
{
    auto&& path = (SCRIPT_DIRECTORY + FileSystem::GetFilePath(name) + SCRIPT_EXTENSION);

    // create scene
    if (const auto script = CreateResource<ScriptInstance>(path))
    {
        script->Update();
        return script;
    }

    return nullptr;
}

bool ScriptInstance::Load(StringView path)
{
    path = path.substr(0, path.find("."));
    m_scriptName = FileSystem::GetFilePath(path);

	SetUpFunctionList();

	return true;
}

void ScriptInstance::Update()
{
	FileStream file(m_filePath, OpenMode::Txt_Mode);
	ASSERT(file.IsOpen());

	Vector<String> initData = {
		"import Engine\n",
		"\n",
		"def Start(this) :\n",
		"    return\n\n",
		"\n",
		"def Update(this, deltaTime) :\n",
		"    return"
	};

	file.Write(initData);
}

void ScriptInstance::CallFunction(Script* script, ScriptFuncType type) noexcept
{
	try
	{
		if (m_hasFunctionList[type])
		{
			m_functionList[type](boost::python::ptr(script));
		}
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
	}
}

void ScriptInstance::CallTickFunction(Script* script, double deltaTime) noexcept
{
	try
	{
		ASSERT(m_hasFunctionList[ScriptFuncType::Update]);
		m_functionList[ScriptFuncType::Update](boost::python::ptr(script), deltaTime);
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
	}
}

bool ScriptInstance::HasTickFunction() const noexcept
{
	return m_hasFunctionList[ScriptFuncType::Update];
}

boost::python::object ScriptInstance::CompileScript(StringView name) noexcept
{
	try
	{
		return boost::python::import(name.data()).attr("__dict__");
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
		return boost::python::object();
	}
}

void ScriptInstance::SetUpFunctionList() noexcept
{
	m_scriptFile = CompileScript(m_scriptName);
	if (m_scriptFile.is_none())
	{
		return;
	}

	SetFunction("Init"		, ScriptFuncType::Init		  , m_scriptFile);
	SetFunction("Register"	, ScriptFuncType::Register    , m_scriptFile);
	SetFunction("UnRegister", ScriptFuncType::UnRegister  , m_scriptFile);
	SetFunction("Start"		, ScriptFuncType::Start		  , m_scriptFile);
	SetFunction("Stop"		, ScriptFuncType::Stop		  , m_scriptFile);
	SetFunction("Remove"	, ScriptFuncType::Remove	  , m_scriptFile);
	SetFunction("Update"	, ScriptFuncType::Update	  , m_scriptFile);
}

void ScriptInstance::ErrorHandle() noexcept
{
	PyObject* type, *value, *traceback;
	PyErr_Fetch(&type, &value, &traceback);

	boost::python::handle<> hexc(type), hval(boost::python::allow_null(value)), htb(boost::python::allow_null(traceback));
	boost::python::object tracebackObject(boost::python::import("traceback"));

	boost::python::object formatted_list;
	if (!traceback)
	{
		boost::python::object format_exception_only(tracebackObject.attr("format_exception_only"));
		formatted_list = format_exception_only(hexc, hval);
	}
	else
	{
		boost::python::object format_exception(tracebackObject.attr("format_exception"));
		formatted_list = format_exception(hexc, hval, htb);
	}

	boost::python::object formatted(boost::python::str("\n").join(formatted_list));
	String errorMsg = boost::python::extract<String>(formatted);

	LOG_ERROR(errorMsg);
}