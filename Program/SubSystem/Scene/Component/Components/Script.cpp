/**
* @file    Script.cpp
* @brief
*
* @date	   2022/11/15 2022îNìxèâî≈
*/


#include "Script.h"
#include <boost/python.hpp>

void Script::Serialized(FileStream* file) const
{
	file->Write(m_scriptName);
}

void Script::Deserialized(FileStream* file)
{
	file->Read(&m_scriptName);

	SetScript(m_scriptName);
}

void Script::OnRegister()
{
	IComponent::OnRegister();
	CallFunction(FuncType::Register);
}

void Script::OnUnRegister()
{
	IComponent::OnUnRegister();
	CallFunction(FuncType::UnRegister);
}

void Script::OnStart()
{
	IComponent::OnStart();
	CallFunction(FuncType::Start);
}

void Script::OnStop()
{
	IComponent::OnStop();
	CallFunction(FuncType::Stop);

	if (m_hasFunctionList[FuncType::Update])
	{
		m_tickFunction.UnRegisterFromTickManager();
	}
}

void Script::OnRemove()
{
	IComponent::OnRemove();
	CallFunction(FuncType::Remove);
}

void Script::Tick(double deltaTime)
{
	m_functionList[FuncType::Update](this, deltaTime);
}

void Script::SetScript(StringView name) noexcept
{
	m_scriptName = name;

	if (!m_scriptName.empty())
	{
		SetUpFunctionList();
	}
}

boost::python::object Script::CompileScript(StringView name) noexcept
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

void Script::SetUpFunctionList() noexcept
{
	boost::python::object scriptFile = CompileScript(m_scriptName);
	if (scriptFile.is_none())
	{
		return;
	}

	GetFunction("Init"		, FuncType::Init	  , scriptFile);
	GetFunction("Register"	, FuncType::Register  , scriptFile);
	GetFunction("UnRegister", FuncType::UnRegister, scriptFile);
	GetFunction("Start"		, FuncType::Start	  , scriptFile);
	GetFunction("Stop"		, FuncType::Stop	  , scriptFile);
	GetFunction("Remove"	, FuncType::Remove	  , scriptFile);
	GetFunction("Update"	, FuncType::Update	  , scriptFile);

	if (m_hasFunctionList[FuncType::Update])
	{
		m_tickFunction.RegisterToTickManager();
	}
}

bool Script::GetFunction(StringView funcName, FuncType type, const boost::python::object& scriptFile) noexcept
{
	if (!scriptFile.contains(funcName))
	{
		return false;
	}

	m_hasFunctionList[type] = true;
	m_functionList[type] = scriptFile[funcName];
	return true;
}

void Script::CallFunction(FuncType type) noexcept
{
	if (m_hasFunctionList[type])
	{
		m_functionList[type](this);
	}
}

void Script::ErrorHandle() noexcept
{
	PyObject* type, * value, * traceback;
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