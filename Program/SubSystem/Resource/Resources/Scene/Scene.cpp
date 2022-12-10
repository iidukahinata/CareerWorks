/**
* @file    Scene.cpp
* @brief
*
* @date	   2022/10/27 2022年度初版
*/


#include "Scene.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Scene/GameObject.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/ResourceData/ProprietarySceneData.h"

Scene::Scene()
{
    m_world = GetContext()->GetSubsystem<World>();
    ASSERT(m_world);
}

Scene::~Scene()
{
    ClearGameObjects();
}

Scene* Scene::Create(StringView name) noexcept
{
    auto&& path = ProprietarySceneData::ConvertProprietaryPath(name);

    // create scene
    if (const auto scene = CreateResource<Scene>(path))
    {
        scene->Update();
        return scene;
    }

    return nullptr;
}

void Scene::Save(StringView path) noexcept
{
    UpdateProprietaryDataFile(path);
}

void Scene::SaveAs(StringView name) noexcept
{
    // original data path
    const auto resourceData = GetResourceData();
    StringView assetFullPath = resourceData->m_assetFullPath;
    StringView resourcePath = resourceData->m_resourcePath.m_path;

    // get file directory
    auto newResourceDirectory = resourcePath.substr(0, resourcePath.find_last_of("/\\") + 1);
    auto newAssetDirectory = assetFullPath.substr(0, assetFullPath.find_last_of("/\\") + 1);

    // new data file path
    auto newResourcePath = newResourceDirectory + name + SCENE_EXTENSION;
    auto newAssetFullPath = newAssetDirectory + name + ASSET_EXTENSION;

    // save as new data
    {
        FileSystem::Copy(resourcePath, newResourcePath);
        const auto newResourceData = m_resourceManager->CreateResourceData<Scene>(newResourcePath, newAssetFullPath);

        newResourceData->m_refResourcePaths = resourceData->m_refResourcePaths;
        m_resourceManager->UpdateResourceData(newResourceData);

        UpdateProprietaryDataFile(newResourcePath);
    }
}

bool Scene::Load(StringView path)
{
    ClearGameObjects();

    FileStream file(path, OpenMode::Read_Mode);
    ASSERT(file.IsOpen());

    size_t numRootGameObjects = 0;
    file.Read(&numRootGameObjects);

    for (int i = 0; i < numRootGameObjects; ++i)
    {
        // this データのゲームオブジェクトを生成
        const auto rootGameObject = m_world->CreateGameObject(this);

        if (rootGameObject)
        {
            rootGameObject->Deserialized(&file);
        }
    }

    return true;
}

void Scene::Update()
{
    UpdateProprietaryDataFile();
    
    if (!m_gameObjects.empty())
    {
        UpdateResourceDataFile();
    }
}

void Scene::AddToWorld() noexcept
{
    auto numGameObject = m_gameObjects.size();
    for (int i = 0; i < numGameObject; ++i)
    {
        m_gameObjects[i]->RegisterAllComponents();
    }

    // 実行中は登録時に再生開始
    if (!m_world->IsGameMode())
    {
        return;
    }

    while (true)
    {
        auto numGameObject = m_gameObjects.size();
        for (int i = 0; i < numGameObject; ++i)
        {
            m_gameObjects[i]->BeginPlay();
        }

        if (m_gameObjects.size() == numGameObject)
        {
            break;
        }
    }
}

void Scene::RemoveFromWorld() noexcept
{
    if (m_world->IsGameMode())
    {
        auto numGameObject = m_gameObjects.size();
        for (int i = 0; i < numGameObject; ++i)
        {
            if (i >= m_gameObjects.size())
                break;

            m_gameObjects[i]->EndPlay();
        }
    }

    auto numGameObject = m_gameObjects.size();
    for (int i = 0; i < numGameObject; ++i)
    {
        if (i >= m_gameObjects.size())
            break;

        m_gameObjects[i]->UnRegisterAllComponents();
    }
}

void Scene::AddGameObject(GameObject* gameObject) noexcept
{
    ASSERT(gameObject);

    gameObject->SetID(m_gameObjects.size());
    m_gameObjects.emplace_back(gameObject);
}

GameObject* Scene::GetGameObjectByID(uint32_t id) const noexcept
{
    ASSERT(0 <= id || id < m_gameObjects.size());

    return m_gameObjects[id].get();
}

GameObject* Scene::GetGameObjectByName(StringView name) const noexcept
{
    for (const auto& gameObject : m_gameObjects)
    {
        if (gameObject->GetName() == name)
        {
            return gameObject.get();
        }
    }
    return nullptr;
}

const Vector<UniquePtr<GameObject>>& Scene::GetAllGameObjects() const noexcept
{
    return m_gameObjects;
}

void Scene::GetAllRootGameObjects(Vector<GameObject*>& gameObjects) const noexcept
{
    const auto numObject = m_gameObjects.size();
    for (int i = 0; i < numObject; ++i)
    {
        if (!m_gameObjects[i])
            continue;

        if (m_gameObjects[i]->GetTransform().HasParent())
            continue;

        gameObjects.emplace_back(m_gameObjects[i].get());
    }
}

void Scene::RemoveGameObject(GameObject* gameObject) noexcept
{
    ASSERT(gameObject);

    gameObject->ClearComponets();

    const auto id = gameObject->GetID();

    ASSERT(0 <= id || id < m_gameObjects.size());
    ASSERT(m_gameObjects[id]->GetName() == gameObject->GetName());

    // O(1)でのデータ入れ替え処理
    m_gameObjects[id].swap(m_gameObjects.back());
    m_gameObjects[id]->SetID(id);

    // 消去が完了していない場合は AutoDestroySystem に渡す
    if (gameObject->RequestAutoDestroy())
    {
        auto autoDestroyObject = m_gameObjects.back().release();
        m_world->GetAutoDestroySystem().AddAutoDestroy(autoDestroyObject);
    }

    m_gameObjects.pop_back();
}

void Scene::ClearGameObjects() noexcept
{
    for (auto& gameObject : m_gameObjects)
    {
        if (!gameObject)
            continue;

        gameObject->ClearComponets();

        if (gameObject->RequestAutoDestroy())
        {
            m_world->GetAutoDestroySystem().AddAutoDestroy(gameObject.release());
        }
    }

    m_gameObjects.clear();
    m_gameObjects.shrink_to_fit();
}

void Scene::UpdateProprietaryDataFile(StringView path /* = StringView() */) noexcept
{
    FileStream file;

    if (path.empty())
    {
        file.Load(m_filePath, OpenMode::Write_Mode);
    }
    else
    {
        file.Load(path, OpenMode::Write_Mode);
    }

    ASSERT(file.IsOpen());

    Vector<GameObject*> rootGameObjects;
    GetAllRootGameObjects(rootGameObjects);

    size_t numRootGameObjects = rootGameObjects.size();
    file.Write(numRootGameObjects);

    for (int i = 0; i < numRootGameObjects; ++i)
    {
        rootGameObjects[i]->Serialized(&file);
    }
}

void Scene::UpdateResourceDataFile() noexcept
{
    auto resourceData = GetResourceData();
    auto& refResourcePaths = resourceData->m_refResourcePaths;

    // 元データの消去
    refResourcePaths.clear();
    refResourcePaths.shrink_to_fit();

    Set<String> refResources;
    for (auto& gameObject : m_gameObjects)
    {
        for (auto& component : gameObject->GetAllComponent())
        {
            component.second->GetUseResourcePaths(refResources);
        }
    }

    for (auto& resource : refResources)
    {
        auto resourceData = m_resourceManager->GetResourceData(resource);
        refResourcePaths.emplace_back(resourceData->m_resourcePath);
    }

    m_resourceManager->UpdateResourceData(resourceData);
}