/**
* @file    ScriptInstance.cpp
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/


#include "ScriptInstance.h"
#include "SubSystem/Scene/Component/IScript.h"
#include "SubSystem/Scene/Component/IRigidBody.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
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
	m_scriptName = FileSystem::GetFilePath(path.substr(0, path.find(".")));

	PostInit();

	return true;
}

void ScriptInstance::Update()
{
	FileStream file(m_filePath, OpenMode::Txt_Mode);
	ASSERT(file.IsOpen());

	String classDef = "class ";
	classDef += FileSystem::GetFilePath(m_filePath.substr(0, m_filePath.find(".")));
	classDef += "(MonoBehaviour.MonoBehaviour) :\n";

	Vector<String> initData = {
		"import Engine\n",
		"import MonoBehaviour\n",
		"\n",
		classDef,
		"\n"
		"	def Start(self) :\n",
		"		return\n",
		"\n",
		"	def Update(self, deltaTime) :\n",
		"		return"
	};

	file.Write(initData);
}

void ScriptInstance::Reload() noexcept
{
	try
	{
		auto importlib  = boost::python::import("importlib");
		auto reloadFunc = importlib.attr("reload");

		reloadFunc(boost::python::import(m_scriptName.data()));

		// get and init from new script data
		PostInit();

		m_eventListenerMap.clear();
		m_classInstanceDataMap.clear();

		// recreate all instance for attach new data
		for (auto script : m_scripts)
		{
			CreateClassInstance(script);
		}
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
	}
}

void ScriptInstance::RegisterScript(IScript* script) noexcept
{
	m_scripts.emplace_back(script);

	CreateClassInstance(script);
}

void ScriptInstance::UnRegisterScript(IScript* script) noexcept
{
	std::erase(m_scripts, script);

	m_classInstanceDataMap.erase(script);
}

bool ScriptInstance::HasTickFunction() const noexcept
{
	return m_hasFunctionList[ScriptFuncType::Update];
}

void ScriptInstance::RegisterEventFunctionList() noexcept
{
	SetupEventFunction<ChangeSceneEvent, Scene*>(ScriptEventFuncType::OnChangeScene);
	SetupEventFunction<LoadSceneCompleteEvent, Scene*>(ScriptEventFuncType::OnLoadSceneComplete);
	SetupEventFunction<KeyPressedEvent , Button::KeyAndMouse>(ScriptEventFuncType::OnKeyPressed);
	SetupEventFunction<KeyReleasedEvent, Button::KeyAndMouse>(ScriptEventFuncType::OnKeyReleased);

	m_registerEventListener = true;
}

void ScriptInstance::UnRegisterEventFunctionList() noexcept
{
	for (auto& eventListener : m_eventListenerMap)
	{
		eventListener.second.UnRegisterFromEventManager();
	}

	m_registerEventListener = false;
}

bool ScriptInstance::IsRegisterEventFunction() const noexcept
{
	return m_registerEventListener;
}

boost::python::object ScriptInstance::GetInstance(IScript* script) const noexcept
{
	if (m_classInstanceDataMap.contains(script))
	{
		return m_classInstanceDataMap.at(script).m_instance;
	}
	return boost::python::object();
}

void ScriptInstance::PostInit() noexcept
{
	m_scriptClass = CompileScript(m_scriptName);
	if (m_scriptClass.is_none())
	{
		return;
	}

	// clear old data
	m_hasFunctionList.fill(false);
	m_hasEventFunctionList.fill(false);

	try
	{
		auto classInfo = m_scriptClass.attr("__dict__");

		// setup function
		InitFunction("Init"			   , ScriptFuncType::Init			 , classInfo);
		InitFunction("Register"		   , ScriptFuncType::Register		 , classInfo);
		InitFunction("UnRegister"	   , ScriptFuncType::UnRegister		 , classInfo);
		InitFunction("Start"		   , ScriptFuncType::Start			 , classInfo);
		InitFunction("Stop"			   , ScriptFuncType::Stop			 , classInfo);
		InitFunction("Remove"		   , ScriptFuncType::Remove			 , classInfo);
		InitFunction("Update"		   , ScriptFuncType::Update			 , classInfo);
		InitFunction("OnCollisionEnter", ScriptFuncType::OnCollisionEnter, classInfo);
		InitFunction("OnCollisionStay" , ScriptFuncType::OnCollisionStay , classInfo);
		InitFunction("OnCollisionExit" , ScriptFuncType::OnCollisionExit , classInfo);
		InitFunction("OnTriggerEnter"  , ScriptFuncType::OnTriggerEnter  , classInfo);
		InitFunction("OnTriggerStay"   , ScriptFuncType::OnTriggerStay   , classInfo);
		InitFunction("OnTriggerExit"   , ScriptFuncType::OnTriggerExit   , classInfo);

		// setup event function
		InitEventFunction("OnKeyPressed"	   , ScriptEventFuncType::OnKeyPressed	     , classInfo);
		InitEventFunction("OnKeyReleased"	   , ScriptEventFuncType::OnKeyReleased		 , classInfo);
		InitEventFunction("OnChangeScene"	   , ScriptEventFuncType::OnChangeScene		 , classInfo);
		InitEventFunction("OnLoadSceneComplete", ScriptEventFuncType::OnLoadSceneComplete, classInfo);
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
	}
}

boost::python::object ScriptInstance::CompileScript(StringView name) noexcept
{
	try
	{
		auto&& scriptmodule = boost::python::import(name.data());
		if (scriptmodule.is_none())
		{
			LOG_ERROR("can't import script module");
		}

		auto&& classModule = scriptmodule.attr(name.data());
		if (scriptmodule.is_none())
		{
			LOG_ERROR("can't get script class");
		}

		return classModule;
	}
	catch (const boost::python::error_already_set&)
	{

		ErrorHandle();

		return boost::python::object();
	}
}

void ScriptInstance::CreateClassInstance(IScript* script) noexcept
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
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();

		return;
	}
}

void ScriptInstance::ErrorHandle() noexcept
{
	PyObject* type, * value, * traceback;
	PyErr_Fetch(&type, &value, &traceback);

	if (value)
	{
		String errorMsg = boost::python::extract<String>(value);
		LOG_ERROR(errorMsg);
	}
}