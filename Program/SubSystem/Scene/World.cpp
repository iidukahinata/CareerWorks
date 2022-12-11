/**
* @file    World.cpp
* @brief
*
* @date	   2022/11/04 2022年度初版
*/


#include "World.h"
#include "SceneEvents.h"
#include "TickManager.h"
#include "Factory/GameObjectFactory.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"

bool World::Initialize()
{
#ifdef IS_EDITOR
	m_worldType = WorldType::Editor;
#else
	m_worldType = WorldType::Game;
#endif // IS_EDITOR

	StartupListenerObjects();

	TickManager::Get().Initialize();

	m_autoDestroySystem.Initialize();

	// ロード完了チェックのため必要時まで登録は行わない
	m_job.SetFunction([this](double) { Update(); }, FunctionType::StartPhysics);

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	return true;
}

void World::Shutdown()
{
	m_job.UnRegisterFromJobSystem();

	m_autoDestroySystem.Shutdown();

	TickManager::Get().Exit();
}

void World::Update() noexcept
{
#ifdef IS_EDITOR
	TIME_LINE_WATCH_START(MainThread, "Component Update");
#endif // IS_EDITOR

	for (auto resourceHandle : m_resourceHandles)
	{
		if (resourceHandle.second->IsValid())
		{
			NotifyEvent<LoadSceneCompleteEvent>(resourceHandle.first);
		}
	}

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(MainThread);
#endif // IS_EDITOR
}

void World::LoadScene(StringView name) noexcept
{
	// loaded
	if (m_sceneList.contains(name))
	{
		NotifyEvent<LoadSceneCompleteEvent>(name);

		return;
	}

	// loading
	if (m_resourceHandles.contains(name))
	{
		return;
	}

	if (auto resourceHandle = m_resourceManager->Load<Scene>(name))
	{
		m_resourceHandles[name] = resourceHandle;

		// for load completion confirmation, it is registered the update function
		m_job.RegisterToJobSystem();
	}
}

void World::UnloadScene(StringView name) noexcept
{
	if (m_sceneList.contains(name))
	{
		RemoveScene(name);

		m_resourceManager->Unload<Scene>(name);
	}
}

void World::ChangeScene(StringView name) noexcept
{
	if (m_sceneList.contains(name))
	{
		if (m_currentScene != m_sceneList[name])
		{
			NotifyEvent<ChangeSceneEvent>(m_sceneList[name]);
		}
	}
	else
	{
		// if not found scene, load a new scene
		LoadScene(name);

		// wait loading and change new scene
		if (auto handle = m_resourceHandles[name])
		{
			handle->WaitForLoadComplete();

			NotifyEvent<ChangeSceneEvent>(handle->GetResource<Scene>());
		}
	}

#ifdef IS_EDITOR
	if (m_currentScene)
	{
		m_currentScene->ClearGameObjects();
	}
#endif // IS_EDITER
}

GameObject* World::CreateGameObject(Scene* scene /* = nullptr */) noexcept
{
	GameObject* result = nullptr;
	auto targetScene = scene ? scene : m_currentScene;
	auto registerObject = (targetScene == m_currentScene);

	ASSERT(targetScene);

	if (auto gameObject = GameObjectFactory::Create(this, targetScene))
	{
		if (registerObject)
		{
			gameObject->RegisterAllComponents();
		}

		result = gameObject.get();
		targetScene->AddGameObject(gameObject.release());

#ifdef IS_EDITOR
		if (m_currentScene)
		{
			CancelEvent<UpdateSceneTreeEvent>(false);
			NotifyEvent<UpdateSceneTreeEvent>();
		}
#endif // IS_EDITOR
	}
	else
	{
		// can't create new gameobject.
		ASSERT(0);
	}

	return result;
}

void World::DestroyGameObject(GameObject* gameObject) const noexcept
{
	ASSERT(gameObject);

	auto scene = gameObject->GetOwner();
	ASSERT(scene);

	scene->RemoveGameObject(gameObject);

#ifdef IS_EDITOR
	if (m_currentScene == scene)
	{
		CancelEvent<UpdateSceneTreeEvent>(false);
		NotifyEvent<UpdateSceneTreeEvent>();
	}
#endif // IS_EDITOR
}

GameObject* World::GetGameObjectByName(StringView name) const noexcept
{
	for (const auto& scene : m_sceneList)
	{
		if (const auto& gameObject = scene.second->GetGameObjectByName(name))
		{
			return gameObject;
		}
	}
	return nullptr;
}

void World::SetCurrentScene(Scene* scene) noexcept
{
	if (m_currentScene)
	{
		m_currentScene->RemoveFromWorld();

#ifdef IS_EDITOR
		if (!IsGameMode())
		{
			UnloadScene(m_currentScene->GetAssetName());
		}
#endif // IS_EDITER
	}

	if (scene)
	{
		scene->AddToWorld();

#ifdef IS_EDITOR
		CancelEvent<UpdateSceneTreeEvent>(false);
		NotifyEvent<UpdateSceneTreeEvent>();
#endif // IS_EDITOR
	}

	m_currentScene = scene;
}

Scene* World::GetCurrentScene() const noexcept
{
	return m_currentScene;
}

void World::AddScene(StringView name, Scene* scene) noexcept
{
	ASSERT(scene);

	if (!m_sceneList.contains(name))
	{
		m_sceneList[name] = scene;
	}
}

void World::RemoveScene(Scene* scene) noexcept
{
	String name;
	for (auto& sceneInfo : m_sceneList)
	{
		if (sceneInfo.second == scene)
		{
			name = sceneInfo.first;
			break;
		}
	}

	if (!name.empty())
	{
		RemoveScene(name);
	}
}

void World::RemoveScene(StringView name) noexcept
{
	if (m_sceneList.contains(name))
	{
		if (m_currentScene == m_sceneList[name])
		{
			m_currentScene = nullptr;

#ifdef IS_EDITOR
			CancelEvent<UpdateSceneTreeEvent>(false);
			NotifyEvent<UpdateSceneTreeEvent>();
#endif // IS_EDITOR
		}

		m_sceneList.erase(name);
	}
}

Scene* World::GetScene(StringView name) noexcept
{
	String sceneName(name);
	if (m_sceneList.contains(sceneName))
	{
		return m_sceneList[sceneName];
	}
	else
	{
		LOG_ERROR("not loading or not load complete so not get scene.");
		return nullptr;
	}
}

const Unordered_Map<String, Scene*>& World::GetSceneList() const noexcept
{
	return m_sceneList;
}

AutoDestroySystem& World::GetAutoDestroySystem() noexcept
{
	return m_autoDestroySystem;
}

bool World::IsGameMode() const noexcept
{
	return m_worldType == WorldType::Game || m_worldType == WorldType::PlayEditor;
}

bool World::IsEditorMode() const noexcept
{
	return m_worldType == WorldType::Editor || m_worldType == WorldType::PlayEditor;
}

void World::StartupListenerObjects() noexcept
{
	m_changeSceneListener.SetFunction([this](std::any data) {

		auto scene = std::any_cast<Scene*>(data);
		SetCurrentScene(scene);

	});

	m_loadSceneCompleteListener.SetFunction([this](std::any data) {

		auto name = std::any_cast<String>(data);

		if (!m_resourceHandles.contains(name))
		{
			return;
		}
		
		if (auto scene = m_resourceHandles[name]->GetResource<Scene>())
		{
			AddScene(name, scene);

			m_resourceHandles.erase(name);
		}

		// if empty check list unregister update function
		if (m_resourceHandles.empty())
		{
			m_job.UnRegisterFromJobSystem();
		}

	});

#ifdef IS_EDITOR
	m_changeEditorStateListener.SetFunction([this](std::any data) {

		constexpr auto tempScenePath = "Data/Temp/tempScene.bin";

		auto editorState = std::any_cast<EditorState>(data);

		switch (editorState)
		{
		case EditorState::Run:
			m_currentScene->Save(tempScenePath);
			ChangeWorldType(WorldType::PlayEditor);
			break;

		case EditorState::Stop:
			ChangeWorldType(WorldType::Editor);
			m_currentScene->Load(tempScenePath);
			break;

		case EditorState::Pause:
			TickManager::Get().Stop();
			break;

		case EditorState::Unpause: 
			TickManager::Get().Start();
			break;

		default: break;
		}

	});

	m_changeEditorStateListener.RegisterToEventManager<ChangeEditorStateEvent>();
#endif // IS_EDITOR

	m_changeSceneListener.RegisterToEventManager<ChangeSceneEvent>();
	m_loadSceneCompleteListener.RegisterToEventManager<LoadSceneCompleteEvent>();
}

void World::ChangeWorldType(WorldType type) noexcept
{
	if (m_currentScene)
	{
		m_currentScene->RemoveFromWorld();
		m_worldType = type;
		m_currentScene->AddToWorld();
	}
	else
	{
		m_worldType = type;
	}
}