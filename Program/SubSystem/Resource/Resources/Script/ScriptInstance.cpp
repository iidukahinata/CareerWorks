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

	PostInit();

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

	CreateClassInstance(script);

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
	if (!m_hasFunctionList[type])
	{
		return;
	}

	try
	{
		auto& instance = m_classInstanceDataMap[script];
		auto& function = instance.m_functionMap[type];

		function();
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
	}
}

void ScriptInstance::CallTickFunction(Script* script, double deltaTime) noexcept
{
	ASSERT(m_hasFunctionList[ScriptFuncType::Update]);

	try
	{
		auto& instance = m_classInstanceDataMap[script];
		auto& function = instance.m_functionMap[ScriptFuncType::Update];

		function(deltaTime);
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
		auto&& scriptmodule = boost::python::import(name.data());
		if (scriptmodule.is_none())
		{
			LOG_ERROR("スクリプトモジュールのインポートに失敗");
		}

		auto&& classModule  = scriptmodule.attr(name.data());
		if (scriptmodule.is_none())
		{
			LOG_ERROR("スクリプトクラス取得に失敗");
		}

		return classModule;
	}
	catch (const boost::python::error_already_set&)
	{

		ErrorHandle();

		return boost::python::object();
	}
}

void ScriptInstance::PostInit() noexcept
{
	m_scriptClass = CompileScript(m_scriptName);
	if (m_scriptClass.is_none())
	{
		return;
	}

	auto classInfo = m_scriptClass.attr("__dict__");

	// clear old data
	m_hasFunctionList.fill(false);
	m_hasEventFunctionList.fill(false);

	// setup function
	InitFunction("Init"		 , ScriptFuncType::Init		 , classInfo);
	InitFunction("Register"	 , ScriptFuncType::Register  , classInfo);
	InitFunction("UnRegister", ScriptFuncType::UnRegister, classInfo);
	InitFunction("Start"	 , ScriptFuncType::Start	 , classInfo);
	InitFunction("Stop"		 , ScriptFuncType::Stop		 , classInfo);
	InitFunction("Remove"	 , ScriptFuncType::Remove	 , classInfo);
	InitFunction("Update"	 , ScriptFuncType::Update	 , classInfo);

	// setup event function
	InitEventFunction("OnKeyPressed"	, ScriptEventFuncType::OnKeyPressed	   , classInfo);
	InitEventFunction("OnKeyReleased"   , ScriptEventFuncType::OnKeyReleased   , classInfo);
	InitEventFunction("OnCollisionEnter", ScriptEventFuncType::OnCollisionEnter, classInfo);
	InitEventFunction("OnCollisionStay" , ScriptEventFuncType::OnCollisionStay , classInfo);
	InitEventFunction("OnCollisionExit" , ScriptEventFuncType::OnCollisionExit , classInfo);
	InitEventFunction("OnTriggerEnter"  , ScriptEventFuncType::OnTriggerEnter  , classInfo);
	InitEventFunction("OnTriggerStay"   , ScriptEventFuncType::OnTriggerStay   , classInfo);
	InitEventFunction("OnTriggerExit"   , ScriptEventFuncType::OnTriggerExit   , classInfo);
	InitEventFunction("OnChangeScene"   , ScriptEventFuncType::OnChangeScene   , classInfo);
}

void ScriptInstance::CreateClassInstance(Script* script) noexcept
{
	try
	{
		// create new object
		auto newInstance = m_scriptClass();

		if (!newInstance.is_none())
		{
			// setup monoBehaviour
			newInstance.attr("_MonoBehaviour__Init")(boost::python::ptr(script));
		}

		m_classInstanceDataMap[script].m_instance = newInstance;

		// register functions
		for (int i = 0; i < ScriptFuncType::MaxFunc; ++i)
		{
			if (!m_hasFunctionList[i])
			{
				continue;
			}

			auto funcName = m_functionNameList[i].c_str();
			m_classInstanceDataMap[script].m_functionMap[i] = newInstance.attr(funcName);
		}

		// register event functions
		for (int i = 0; i < ScriptEventFuncType::MaxEventFunc; ++i)
		{
			if (!m_hasEventFunctionList[i])
			{
				continue;
			}

			auto funcName = m_eventFunctionNameList[i].c_str();
			m_classInstanceDataMap[script].m_eventFunctionMap[i] = newInstance.attr(funcName);
		}
	}
	catch (const std::exception&)
	{
		ErrorHandle();

		return;
	}
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