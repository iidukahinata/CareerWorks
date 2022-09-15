/**
* @file    World.cpp
* @brief
*
* @date	   2022/09/05 2022�N�x����
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
			const auto& name = resourceHandle.first;
			const auto& scene = resourceHandle.second->GetResource<Scene>();

			AddScene(name, scene);
			NotifyEvent<LoadSceneCompleteEvent>(name);
		}
	}
}

void World::LoadScene(StringView name) noexcept
{
	String sceneName(name);

	if (m_sceneList.contains(sceneName))
	{
		NotifyEvent<LoadSceneCompleteEvent>(sceneName);
		return;
	}

	if (m_resourceHandles.contains(sceneName))
	{
		return;
	}

	m_resourceHandles[sceneName] = m_resourceManager->Load<Scene>(name);

	// ���[�h�����`�F�b�N�p�ɍX�V�֐��̓o�^
	m_job.RegisterToJobSystem();
}

void World::UnloadScene(StringView name) noexcept
{
	if (m_sceneList.contains(name.data()))
	{
		RemoveScene(name);
		m_resourceManager->Unload(Scene::TypeData.Hash, name);
	}
	else
	{
		LOG_ERROR("�w�薼�� Scene �͓o�^����Ă��܂���ł���");
	}
}

void World::ChangeScene(StringView name) noexcept
{
	String sceneName(name);

	if (m_sceneList.contains(sceneName) && m_sceneList[sceneName])
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
		scene->RemoveGameObject(gameObject);
	}
}

GameObject* World::GetGameObjectByName(StringView name) const noexcept
{
	for (const auto& scene : m_sceneList)
	{
		if (auto gameObject = scene.second->GetGameObjectByName(name))
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

		auto resourceData = m_resourceManager->GetResourceData(m_currentScene->GetFilePath());
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
		LOG_ERROR("Load ����Ă��Ȃ� / �������Ă��Ȃ����ߎ擾�ł��܂���ł����B");
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

		auto sceneName = std::any_cast<String>(data);
		m_resourceHandles.erase(sceneName);

		// ���ʂȍX�V����������Ȃ��悤�ɓo�^����
		if (m_resourceHandles.empty())
		{
			m_job.UnRegisterFromJobSystem();
		}

	});

	m_changeSceneListener.RegisterToEventManager<ChangeSceneEvent>();
	m_loadSceneCompleteListener.RegisterToEventManager<LoadSceneCompleteEvent>();
}

void World::AddScene(StringView name, Scene* scene) noexcept
{
	String sceneName(name);

	if (!m_sceneList.contains(sceneName))
	{
		m_sceneList[sceneName] = scene;
	}
}

void World::RemoveScene(Scene* scene) noexcept
{
	StringView name;

	for (auto& sceneInfo : m_sceneList)
	{
		if (sceneInfo.second == scene)
		{
			name = sceneInfo.first;
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

	if (m_sceneList.contains(sceneName))
	{
		m_sceneList.erase(sceneName);
	}
}