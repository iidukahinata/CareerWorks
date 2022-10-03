/**
* @file    World.cpp
* @brief
*
* @date	   2022/10/02 2022�N�x����
*/


#include "World.h"
#include "SceneEvents.h"
#include "TickManager.h"
#include "Factory/GameObjectFactory.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"

bool World::Initialize()
{
	TickManager::Get().Initialize();

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);

	// ���[�h�����`�F�b�N�̂��ߕK�v���܂œo�^�͍s��Ȃ�
	m_job.SetFunction([this](double) { Update(); }, FunctionType::Update);

	// SceneEvent �p���X�i�[�֐��̏�����
	SetUpListenerObjects();

	return true;
}

void World::Shutdown()
{
	TickManager::Get().Exit();
}

void World::Update() noexcept
{
	for (auto resourceHandle : m_resourceHandles)
	{
		if (resourceHandle.second->IsValid())
		{
			NotifyEvent<LoadSceneCompleteEvent>(resourceHandle.first);
		}
	}
}

void World::LoadScene(StringView name) noexcept
{
	String sceneName(name);

	// Load�ς�
	if (m_sceneList.contains(sceneName))
	{
		NotifyEvent<LoadSceneCompleteEvent>(sceneName);
		return;
	}

	// Load��
	if (m_resourceHandles.contains(sceneName))
	{
		return;
	}

	if (auto&& resourceHandle = m_resourceManager->Load<Scene>(name))
	{
		m_resourceHandles[sceneName] = resourceHandle;

		// ���[�h�����`�F�b�N�p�ɍX�V�֐��̓o�^
		m_job.RegisterToJobSystem();
	}
}

void World::UnloadScene(StringView name) noexcept
{
	String sceneName(name);
	if (m_sceneList.contains(sceneName))
	{
		RemoveScene(name);
		m_resourceManager->Unload<Scene>(name);
	}
	else
	{
		LOG_ERROR("Scene �͓o�^����Ă��܂���ł��� : �w�薼 => " + sceneName);
	}
}

void World::ChangeScene(StringView name) noexcept
{
	String sceneName(name);
	if (m_sceneList.contains(sceneName))
	{
		if (m_currentScene != m_sceneList[sceneName]) 
		{
			NotifyEvent<ChangeSceneEvent>(m_sceneList[sceneName]);
		}
	}
	else
	{
		// �w��V�[���������ꍇ�̓V�[�����[�h���s���B
		LoadScene(name);

		auto& handle = m_resourceHandles[sceneName];
		handle->WaitForLoadComplete();

		// �ǂݍ��񂾃V�[���ɕύX
		NotifyEvent<ChangeSceneEvent>(handle->GetResource<Scene>());
	}
}

GameObject* World::CreateGameObject(Scene* scene /* = nullptr */) noexcept
{
	GameObject* result = nullptr;

	auto targetScene = scene ? scene : m_currentScene;
	ASSERT(targetScene);

	if (auto gameObject = GameObjectFactory::Create(this, targetScene))
	{
		result = gameObject.get();
		targetScene->AddGameObject(gameObject.release());
	}

	return result;
}

void World::DestroyGameObject(GameObject* gameObject) noexcept
{
	if (gameObject)
	{
		auto scene = gameObject->GetOwner();
		ASSERT(scene);
		scene->RemoveGameObject(gameObject);
	}
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
	// ���݃V�[���̉��
	if (m_currentScene)
	{
		m_currentScene->RemoveFromWorld();

#ifdef IS_EDITOR
		RemoveScene(m_currentScene);

		const auto resourceData = m_resourceManager->GetResourceData(m_currentScene->GetFilePath());
		m_resourceManager->Unload(resourceData);
#endif // IS_EDITER
	}

	if (scene)
	{
		scene->AddToWorld();
	}

	m_currentScene = scene;
}

Scene* World::GetCurrentScene() const noexcept
{
	return m_currentScene;
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
		LOG_ERROR("Load ����Ă��Ȃ� / �������Ă��Ȃ����� Scene �擾�ł��܂���B");
		return nullptr;
	}
}

const Unordered_Map<String, Scene*>& World::GetSceneList() const noexcept
{
	return m_sceneList;
}

void World::SetUpListenerObjects() noexcept
{
	m_changeSceneListener.SetFunction([this](std::any data) {

		auto scene = std::any_cast<Scene*>(data);
		SetCurrentScene(scene);

	});

	m_loadSceneCompleteListener.SetFunction([this](std::any data) {

		auto name = std::any_cast<String>(data);
		auto scene = m_resourceHandles[name]->GetResource<Scene>();

		AddScene(name, scene);

		m_resourceHandles.erase(name);
		if (m_resourceHandles.empty())
		{
			// ���ʂȍX�V����������Ȃ��悤�ɓo�^����
			m_job.UnRegisterFromJobSystem();
		}

	});

	m_changeSceneListener.RegisterToEventManager<ChangeSceneEvent>();
	m_loadSceneCompleteListener.RegisterToEventManager<LoadSceneCompleteEvent>();
}

void World::AddScene(StringView name, Scene* scene) noexcept
{
	ASSERT(scene);

	String sceneName(name);
	ASSERT(!m_sceneList.contains(sceneName));
	m_sceneList[sceneName] = scene;
}

void World::RemoveScene(Scene* scene) noexcept
{
	StringView name;
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
	String sceneName(name);
	ASSERT(m_sceneList.contains(sceneName));
	m_sceneList.erase(sceneName);
}