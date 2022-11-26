/**
* @file    ScriptInstance.cpp
* @brief
*
* @date	   2022/11/20 2022年度初版
*/


#include "ScriptInstance.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Scene/Component/Components/Script.h"
#include "SubSystem/Scene/Component/Components/RigidBody.h"
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

	if (!m_scripts.empty())
	{
		// 関数の設定し直す必要がある
		SetUpEventFunctionLisyt();
	}

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
		"    return\n",
		"\n",
		"def Update(this, deltaTime) :\n",
		"    return"
	};

	file.Write(initData);
}

void ScriptInstance::RegisterScript(Script* script) noexcept
{
	m_scripts.emplace_back(script);

	// 関数の設定し直す必要がある
	SetUpEventFunctionLisyt();
}

void ScriptInstance::UnRegisterScript(Script* script) noexcept
{
	std::erase(m_scripts, script);

	// 関数の設定し直す必要がある
	SetUpEventFunctionLisyt();
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

const String& ScriptInstance::GetScriptName() const noexcept
{
	return m_scriptName;
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

	// clear old data
	{
		m_eventFuncList.clear();
		m_eventListenerList.clear();
		m_hasFunctionList.fill(false);
		m_functionList.fill(boost::python::object());
	}

	// setup function
	SetFunction("Init"		, ScriptFuncType::Init		  , m_scriptFile);
	SetFunction("Register"	, ScriptFuncType::Register    , m_scriptFile);
	SetFunction("UnRegister", ScriptFuncType::UnRegister  , m_scriptFile);
	SetFunction("Start"		, ScriptFuncType::Start		  , m_scriptFile);
	SetFunction("Stop"		, ScriptFuncType::Stop		  , m_scriptFile);
	SetFunction("Remove"	, ScriptFuncType::Remove	  , m_scriptFile);
	SetFunction("Update"	, ScriptFuncType::Update	  , m_scriptFile);

	// setup event function
	SetEventFunction("OnKeyPressed"	   , ScriptEventFuncType::OnKeyPressed	  , m_scriptFile);
	SetEventFunction("OnKeyReleased"   , ScriptEventFuncType::OnKeyReleased	  , m_scriptFile);
	SetEventFunction("OnCollisionEnter", ScriptEventFuncType::OnCollisionEnter, m_scriptFile);
	SetEventFunction("OnCollisionStay" , ScriptEventFuncType::OnCollisionStay , m_scriptFile);
	SetEventFunction("OnCollisionExit" , ScriptEventFuncType::OnCollisionExit , m_scriptFile);
	SetEventFunction("OnTriggerEnter"  , ScriptEventFuncType::OnTriggerEnter  , m_scriptFile);
	SetEventFunction("OnTriggerStay"   , ScriptEventFuncType::OnTriggerStay	  , m_scriptFile);
	SetEventFunction("OnTriggerExit"   , ScriptEventFuncType::OnTriggerExit   , m_scriptFile);
	SetEventFunction("OnChangeScene"   , ScriptEventFuncType::OnChangeScene	  , m_scriptFile);
}

void ScriptInstance::SetUpEventFunctionLisyt() noexcept
{
	SetUpEventFunction<KeyPressedEvent, Button::KeyAndMouse>(ScriptEventFuncType::OnKeyPressed);
	SetUpEventFunction<KeyReleasedEvent, Button::KeyAndMouse>(ScriptEventFuncType::OnKeyReleased);
	SetUpEventFunction<CollisionEnterEvent, RigidBody*>(ScriptEventFuncType::OnCollisionEnter);
	SetUpEventFunction<CollisionStayEvent, RigidBody*>(ScriptEventFuncType::OnCollisionStay);
	SetUpEventFunction<CollisionExitEvent, RigidBody*>(ScriptEventFuncType::OnCollisionExit);
	SetUpEventFunction<TriggerEnterEvent, RigidBody*>(ScriptEventFuncType::OnTriggerEnter);
	SetUpEventFunction<TriggerStayEvent, RigidBody*>(ScriptEventFuncType::OnTriggerStay);
	SetUpEventFunction<TriggerExitEvent, RigidBody*>(ScriptEventFuncType::OnTriggerExit);
	SetUpEventFunction<ChangeSceneEvent, Scene*>(ScriptEventFuncType::OnChangeScene);
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