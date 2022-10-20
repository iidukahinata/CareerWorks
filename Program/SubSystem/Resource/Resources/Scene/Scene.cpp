/**
* @file    Scene.cpp
* @brief
*
* @date	   2022/09/06 2022年度初版
*/


#include "Scene.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Scene/GameObject.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/ResourceData/ProprietarySceneData.h"

Scene::~Scene()
{
    RemoveFromWorld();
}

Scene* Scene::Create(StringView name) noexcept
{
    auto path = ProprietarySceneData::ConvertProprietaryPath(name);

    auto scene = CreateResource<Scene>(path);

    scene->Update();
    
    return scene;
}

bool Scene::Load(StringView path)
{
    const auto world = GetContext()->GetSubsystem<World>();
    ASSERT(world);

    FileStream file(path, OpenMode::Read_Mode);
    ASSERT(file.IsOpen());

    size_t numRootGameObjects;
    file.Read(&numRootGameObjects);

    for (int i = 0; i < numRootGameObjects; ++i)
    {
        // this データのゲームオブジェクトを生成
        auto rootGameObject = world->CreateGameObject(this);

        rootGameObject->Deserialization(&file);
    }

    return true;
}

void Scene::Update()
{
    UpdateProprietaryDataFile();
    
    // 生成時は、リソースデータは更新なし
    if (!m_gameObjects.empty())
    {
        UpdateResourceDataFile();
    }
}

void Scene::AddToWorld() noexcept
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->StartAllComponents();
    }
}

void Scene::RemoveFromWorld() noexcept
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->StopAllComponents();
    }
}

void Scene::Clone(StringView name) noexcept
{
    // original data
    const auto resourceData = GetResourceData();
    StringView assetFullPath = resourceData->m_assetFullPath;
    StringView resourcePath  = resourceData->m_resourcePath.m_path;

    // clone data
    auto newResourceDirectory = resourcePath.substr(0, resourcePath.find_last_of("/\\") + 1);
    auto newAssetFullDirectory = assetFullPath.substr(0, assetFullPath.find_last_of("/\\") + 1);

    String newResourcePath = String(newResourceDirectory) + name.data() + SCENE_EXTENSION;
    String newAssetFullPath = String(newAssetFullDirectory) + name.data() + ASSET_EXTENSION;

    // clone
    FileSystem::Copy(resourcePath, newResourcePath);
    const auto newResourceData = m_resourceManager->CreateResourceData<Scene>(newResourcePath, newAssetFullPath);

    newResourceData->m_refResourcePaths = resourceData->m_refResourcePaths;
    m_resourceManager->UpdateResourceData(newResourceData);
}

void Scene::AddGameObject(GameObject* gameObject) noexcept
{
    if (gameObject)
    {
        gameObject->SetID(m_gameObjects.size());
        m_gameObjects.emplace_back(gameObject);
    }
}

GameObject* Scene::GetGameObjectByID(uint32_t id) const noexcept
{
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

void Scene::RemoveGameObject(GameObject* gameObject) noexcept
{
    const auto id = gameObject->GetID();

    // アクセス範囲を超えないため
    ASSERT(0 <= id || id < m_gameObjects.size());

    // ID値が上書きされていないことを保証
    ASSERT(m_gameObjects[id]->GetName() == gameObject->GetName());

    // O(1)でのデータ入れ替え処理
    m_gameObjects[id].swap(m_gameObjects.back());

    m_gameObjects[id]->SetID(id);
    m_gameObjects.pop_back();
}

void Scene::ClearGameObjects() noexcept
{
    m_gameObjects.clear();
    m_gameObjects.shrink_to_fit();
}

void Scene::GetAllRootGameObjects(Vector<GameObject*>& gameObjects) const noexcept
{
    for (const auto& gameObject : m_gameObjects)
    {
        if (gameObject->GetTransform().HasParent())
            continue;

        gameObjects.emplace_back(gameObject.get());
    }
}

const Vector<UniquePtr<GameObject>>& Scene::GetAllGameObjects() const noexcept
{
    return m_gameObjects;
}

void Scene::UpdateProprietaryDataFile() noexcept
{
    FileStream file(m_filePath, OpenMode::Write_Mode);
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

    // Eidter 上では一つのレベルしかロードされないため
    auto&& resources = m_resourceManager->GetResources();

    // 自身を参照リソースに入れないため
    refResourcePaths.reserve(resources.size() - 1);
    for (auto resource : resources)
    {
        if (resource == this)
            continue;

        refResourcePaths.push_back(ResourcePath(resource->GetType(), resource->GetFilePath()));
    }

    m_resourceManager->UpdateResourceData(resourceData);
}