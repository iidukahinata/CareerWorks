/**
* @file    ResourceManager.cpp
* @brief
*
* @date	   2022/10/21 2022年度初版
*/


#include "ResourceManager.h"
#include "Factory/ResourceFactory.h"

bool ResourceManager::Initialize()
{
#ifdef IS_EDITOR
    // create importer
    m_modelImporter   = std::make_unique<ModelImporter>(this);
    m_textureImporter = std::make_unique<TextureImporter>(this);
    m_shaderImporter  = std::make_unique<ShaderImporter>(this);
    m_audioImporter   = std::make_unique<AudioImporter>(this);

    ImportAssets();

    StartupListenerObjects();
#endif // IS_EDITOR

    DependencyBuilding();

    m_resourceCache = std::make_unique<ResourceCache>();

    return true;
}

void ResourceManager::Shutdown()
{
    for (const auto& job : m_asyncJobs)
    {
        job->WaitForFinish();
    }

    m_resourceCache->Clear();
}

IResource* ResourceManager::CreateResource(StringView type, StringView path) noexcept
{
    return CreateResource(GetHashFromCRC(type), path);
}

IResource* ResourceManager::CreateResource(uint32_t type, StringView path) noexcept
{
    IResource* result = nullptr;

    if (const auto resourceData = CreateResourceData(type, path))
    {
        auto resource = ResourceFactory::Create(type, path, this);
        ASSERT(resource);

        result = resource.get();
        m_resourceCache->AddResource(resourceData->m_resourcePath, resource.release());
    }

    return result;
}

ResourceHandle* ResourceManager::Load(StringView type, StringView name, uint32_t priority /* = DefaltPriority */) noexcept
{
    return Load(GetHashFromCRC(type), name, priority);
}

ResourceHandle* ResourceManager::Load(uint32_t type, StringView name, uint32_t priority /* = DefaltPriority */) noexcept
{
    auto resourceData = GetResourceData(type, name);
    if (!resourceData)
    {
        LOG_ERROR("指定された ResourceData がなかったため、リソース Load に失敗しました。\n ファイル名 : " + String(name));
        return nullptr;
    }

    Load(resourceData, priority);
}

ResourceHandle* ResourceManager::Load(ResourceData* resourceData, uint32_t priority /* = DefaltPriority */) noexcept
{
    // 既に読み込まれている場合の確認
    if (m_resourceHandles.contains(resourceData))
    {
        return m_resourceHandles[resourceData].get();
    }

    // ユーザー用 Handle の生成
    auto resourceHandle = new ResourceHandle();
    resourceHandle->m_priority        = priority;
    resourceHandle->m_resourceData    = resourceData;
    resourceHandle->m_resourceManager = this;

    m_resourceHandles[resourceData].reset(resourceHandle);

    // 同時読み込みを行うリスト
    Vector<ResourcePath> resourcePaths;
    Set<ResourceData*> addResourceList;

    // 指定リソースと参照関係のリソースも同時に読み込みするためリスト化
    CreateAsyncLoadList(resourceData, resourcePaths, addResourceList);

    // Unload時使用する参照数をカウント
    for (const auto& resource : addResourceList)
    {
        resource->m_numLoading++;
    }

    AsyncLoad(resourceHandle, resourcePaths);

    return resourceHandle;
}

void ResourceManager::Unload(StringView type, StringView name) noexcept
{
    Unload(GetHashFromCRC(type), name);
}

void ResourceManager::Unload(uint32_t type, StringView name) noexcept
{
    auto resourceData = GetResourceData(type, name);
    if (!resourceData)
    {
        LOG_ERROR("指定された ResourceData がなかったため、リソース Unload に失敗しました。\n ファイル名 : " + String(name));
        return;
    }

    Unload(resourceData);
}

void ResourceManager::Unload(ResourceData* resourceData) noexcept
{
    const auto isLoad = m_resourceHandles.contains(resourceData);
    const auto isCompleteLoad = isLoad ? m_resourceHandles[resourceData]->IsValid() : false;

    // 使用されていない / ロードが完了していない場合に Unload 処理に入らないため
    if (isLoad && isCompleteLoad)
    {
        // 同時読み込みを行ったリスト作成
        Vector<ResourcePath> resourcePaths;
        Set<ResourceData*> addResourceList;

        CreateAsyncLoadList(resourceData, resourcePaths, addResourceList);

        // リストの参照カウントを現象させる
        for (const auto& resource : addResourceList)
        {
            resource->m_numLoading--;
            if (resource->m_numLoading == 0)
            {
                m_resourceCache->RemoveResource(resource->m_resourcePath);
            }
        }

        m_resourceHandles.erase(resourceData);
    }
    else if (m_resourceCache->HasResource(resourceData->m_resourcePath))
    {
        m_resourceCache->RemoveResource(resourceData->m_resourcePath);
    }
}

IResource* ResourceManager::GetResource(StringView type, StringView name) noexcept
{
    return GetResource(GetResourceData(GetHashFromCRC(type), name));
}

IResource* ResourceManager::GetResource(uint32_t type, StringView name) noexcept
{
    return GetResource(GetResourceData(type, name));
}

IResource* ResourceManager::GetResource(ResourceData* resourceData) const noexcept
{
    if (resourceData)
    {
        return GetResource(resourceData->m_resourcePath);
    }
    return nullptr;
}

IResource* ResourceManager::GetResource(const ResourcePath& resourcePath) const noexcept
{
    if (const auto resource = m_resourceCache->GetResource(resourcePath))
    {
        return resource;
    }
    return nullptr;
}

Vector<IResource*> ResourceManager::GetResourcesByType(StringView type) noexcept
{
    return GetResourcesByType(GetHashFromCRC(type));
}

Vector<IResource*> ResourceManager::GetResourcesByType(uint32_t type) const noexcept
{
    return m_resourceCache->GetResourcesByType(type);
}

Vector<IResource*> ResourceManager::GetResources() const noexcept
{
    return m_resourceCache->GetResources();
}

ResourceData* ResourceManager::CreateResourceData(StringView type, StringView path, StringView assetPath /* = StringView() */) noexcept
{
    return CreateResourceData(GetHashFromCRC(type), path, assetPath);
}

ResourceData* ResourceManager::CreateResourceData(uint32_t type, StringView path, StringView assetPath /* = StringView() */) noexcept
{
    String filePath(path);
    String assetFullPath(assetPath);

    if (assetFullPath.empty())
    {
        assetFullPath = ToAssetDirectory(path);
    }

    // 生成する asset ファイルパスの作成
    auto noExtPath = path.substr(0, path.find("."));
    auto assetName = FileSystem::GetFilePath(noExtPath);

    // 二つ同じデータが存在しないようにするための判定
    if (GetResourceData(type, assetName))
    {
        LOG_ERROR("既に同じ名前の ResourceData が存在しました。");
        return nullptr;
    }

    auto& resourceData = m_resourceTypeList[type][assetName];
    resourceData.m_resourcePath.m_type = type;
    resourceData.m_resourcePath.m_path = filePath;
    resourceData.m_assetName           = assetName;
    resourceData.m_assetFullPath       = assetFullPath;

    // .asset ファイルデータの作成と初期化
    UpdateResourceData(&resourceData);

    // パスからのアクセス用変数も作成
    m_resourceDataList[filePath] = &resourceData;
    return m_resourceDataList[filePath];
}

void ResourceManager::UpdateResourceData(ResourceData* resourceData) noexcept
{
    if (!resourceData)
    {
        return;
    }

    // asset data 取得
    const auto& resourcePath = resourceData->m_resourcePath;
    const auto& type         = resourcePath.m_type;
    const auto& assetName    = resourceData->m_assetName;
    const auto& assetPath    = resourceData->m_assetFullPath;

    FileStream file;
    if (file.CreateFileAndLoad(assetPath, OpenMode::Write_Mode))
    {
        file.Write(type);
        file.Write(assetName);
        resourceData->Serialized(&file);
    }
    else
    {
        LOG_ERROR("asset データの更新処理に失敗しました。\n ファイル名 : ");
    }
}

ResourceData* ResourceManager::GetResourceData(StringView path) noexcept
{
    String filePath(path);

    if (m_resourceDataList.contains(filePath))
    {
        return m_resourceDataList[filePath];
    }

    //LOG_ERROR("指定パスと該当する ResourceData はありませんでした。\n ファイル名 : " + String(filePath));
    return nullptr;
}

void ResourceManager::GetResourceDataListFromDirectory(StringView path, Vector<std::pair<String, ResourceData*>>& resourceDatas) noexcept
{
    auto&& filePaths = FileSystem::GetFilePathsFromDirectory(path);

    resourceDatas.resize(filePaths.size());

    // 指定ディレクトリには .asset ファイルしか存在しないとして処理
    for (int i = 0; i < filePaths.size(); ++i)
    {
        FileStream file;
        file.Load(filePaths[i], OpenMode::Read_Mode);

        ASSERT(file.IsOpen());

        uint32_t type;
        file.Read(&type);

        String assetName;
        file.Read(&assetName);

        resourceDatas[i] = std::make_pair(assetName, GetResourceData(type, assetName));
    }
}

ResourceData* ResourceManager::GetResourceData(StringView type, StringView name) noexcept
{
    return GetResourceData(GetHashFromCRC(type), name);
}

ResourceData* ResourceManager::GetResourceData(uint32_t type, StringView name) noexcept
{
    if (m_resourceTypeList.contains(type))
    {
        String assetName(name);

        if (m_resourceTypeList[type].contains(assetName))
        {
            return &m_resourceTypeList[type][assetName];
        }
    }

    return nullptr;
}

void ResourceManager::ImportAssets() noexcept
{
    auto&& filePaths = FileSystem::GetFilePathsRecursiveDirectory(IMPORT_ASSET_DIRECTORY);

    ImportAssets(filePaths);
}

void ResourceManager::ImportAssets(const Vector<String>& filePaths) noexcept
{
    for (int i = 0; i < filePaths.size(); ++i)
    {
        StringView path = filePaths[i];
        if (IsProprietaryFilePath(path))
        {
            continue;
        }

        bool ret = false;
        String directory = ORIGINAL_DATA_DIRECTORY;

        // 次回呼び出し時のために独自データを作成
        if (IsModelFilePath(path))
        {
            ret = m_modelImporter->CreateModelData(path);
            directory = ORIGINAL_MODEL_DIRECTORY;
        }
        else if (IsTextureFilePath(path))
        {
            ret = m_textureImporter->CreateTextureData(path);
            directory = ORIGINAL_TEXTURE_DIRECTORY;
        }
        else if (IsAudioFilePath(path))
        {
            ret = m_audioImporter->CreateShaderData(path);
            directory = ORIGINAL_AUDIO_DIRECTORY;
        }
        else if (IsShaderFilePath(path))
        {
            ret = m_shaderImporter->CreateShaderData(path);
            directory = ORIGINAL_SHADER_DIRECTORY;
        }

        // 上書きしないようオリジナルデータとして保存
        if (ret)
        {
            FileSystem::Rename(path, directory + FileSystem::GetFilePath(path));
        }
    }
}

Vector<uint32_t> ResourceManager::GetResourceTypes() const noexcept
{
    Vector<uint32_t> types;

    types.reserve(m_resourceTypeList.size());
    for (auto& resourceList : m_resourceTypeList)
    {
        types.push_back(resourceList.first);
    }

    return types;
}

const Map<uint32_t, Unordered_Map<String, ResourceData>>& ResourceManager::GetAllResourceData() const noexcept
{
    return m_resourceTypeList;
}

const Unordered_Map<String, ResourceData>& ResourceManager::GetResourceDataListByType(uint32_t type) const noexcept
{
    if (m_resourceTypeList.contains(type))
    {
        return m_resourceTypeList.at(type);
    }
    return Unordered_Map<String, ResourceData>();
}

#ifdef IS_EDITOR
void ResourceManager::StartupListenerObjects() noexcept
{    
    m_deleteObjectListener.SetFunction([this](std::any data) {

        auto DeleteResource = [this](ResourceData* resourceData) {

            const auto assetName     = resourceData->m_assetName;
            const auto assetFullPath = resourceData->m_assetFullPath;
            const auto type          = resourceData->m_resourcePath.m_type;
            const auto fullPath      = resourceData->m_resourcePath.m_path;

            Unload(resourceData);
            m_resourceDataList.erase(fullPath);
            m_resourceTypeList[type].erase(assetName);

            FileSystem::RemoveAll(fullPath);
            FileSystem::RemoveAll(assetFullPath);

        };

        const auto path = std::any_cast<String>(data);
        if (FileSystem::IsDirectoryPath(path))
        {
            Vector<std::pair<String, ResourceData*>> resourceDatas;
            GetResourceDataListFromDirectory(path, resourceDatas);

            for (const auto& resourceData : resourceDatas)
            {
                DeleteResource(resourceData.second);
            }

            FileSystem::RemoveAll(path);
        }
        else
        {
            DeleteResource(m_resourceDataList[path]);
        }

    });

    m_importResourceListener.SetFunction([this](std::any data) {

        auto path = std::any_cast<String>(data);

        Vector<String> filePaths;
        if (FileSystem::IsDirectoryPath(path))
        {
            filePaths = FileSystem::GetFilePathsRecursiveDirectory(path);
        }
        else
        {
            filePaths.emplace_back(path);
        }

        ImportAssets(filePaths);

    });

    m_deleteObjectListener.RegisterToEventManager<DeleteObjectEvent>();
    m_importResourceListener.RegisterToEventManager<ImportResourceEvent>();
}
#endif // IS_EDITOR

void ResourceManager::DependencyBuilding() noexcept
{
    m_resourceTypeList.clear();
    m_resourceDataList.clear();

    auto&& filePaths = FileSystem::GetFilePathsRecursiveDirectory(ASSET_DATA_DIRECTORY);

    // 指定ディレクトリには .asset ファイルしか存在しないとして処理
    // アセットファイルから ResourceData の静的構築を行う。
    for (int i = 0; i < filePaths.size(); ++i)
    {
        const auto& path = filePaths[i];

        FileStream file;
        file.Load(path, OpenMode::Read_Mode);

        ASSERT(file.IsOpen());

        uint32_t type;
        file.Read(&type);

        String assetName;
        file.Read(&assetName);

        ResourceData resourceData;
        resourceData.m_assetName     = assetName;
        resourceData.m_assetFullPath = path;
        resourceData.Deserialized(&file);

        m_resourceTypeList[type][assetName] = resourceData;

        // 直接パスからのアクセス用変数も作成
        const auto& resourcePath = resourceData.m_resourcePath.m_path;
        m_resourceDataList[resourcePath] = &m_resourceTypeList[type][assetName];
    }
}

void ResourceManager::CreateAsyncLoadList(ResourceData* resourceData, Vector<ResourcePath>& resourcePaths, Set<ResourceData*>& addResourceList) noexcept
{
    addResourceList.insert(resourceData);

    // 参照リソース Load の読み込みが完了しているために参照と逆順でリスト作成
    const auto& refResourcePaths = resourceData->m_refResourcePaths;
    for (const auto& resourcePath : refResourcePaths)
    {
        const auto& data = GetResourceData(resourcePath.m_path);
        if (!addResourceList.contains(data))
        {
            CreateAsyncLoadList(data, resourcePaths, addResourceList);
        }
    }

    resourcePaths.emplace_back(resourceData->m_resourcePath);
}

void ResourceManager::AsyncLoad(ResourceHandle* resourceHandle, const Vector<ResourcePath>& resourcePaths) noexcept
{
    m_asyncJobs.emplace_back(std::make_unique<AsyncJob>());
    auto& asyncJob = m_asyncJobs.back();

    // Async Load 処理セット
    asyncJob->SetFunction([this, resourceHandle, resourcePaths]() {

        // リソース読み込みループ
        for (int i = 0; i < resourcePaths.size(); ++i)
        {
            const auto type = resourcePaths[i].m_type;
            const auto path = resourcePaths[i].m_path;

            // 同じデータが二つメモリに存在しないようにする
            if (!!(m_resourceCache->GetResource(resourcePaths[i])))
            {
                continue;
            }

            auto resource = ResourceFactory::Create(type, path, this);
            ASSERT(resource);

            if (resource->Load(path))
            {
                m_resourceCache->AddResource(resourcePaths[i], resource.release());
            }
            else
            {
                LOG_ERROR("Resource Load に失敗しました。\n　ファイル名 : " + path);
            }
        }

        resourceHandle->NotifyCompleteLoad();
    });

    // スレッドタスクとして追加
    asyncJob->RegisterToJobSystem();
}

#define CASE_EXT_RETURN(EXT) case GetHashFromCRC(EXT): return true;

bool ResourceManager::IsModelFilePath(StringView path) const noexcept
{
    switch (GetHashFromCRC(GetExt(path)))
    {
    CASE_EXT_RETURN("obj")
    CASE_EXT_RETURN("3ds")
    CASE_EXT_RETURN("max")
    CASE_EXT_RETURN("6kt")
    CASE_EXT_RETURN("6ks")
    CASE_EXT_RETURN("rok")
    CASE_EXT_RETURN("ac")
    CASE_EXT_RETURN("c3d")
    CASE_EXT_RETURN("cob")
    CASE_EXT_RETURN("xsi")
    CASE_EXT_RETURN("fbx")
    CASE_EXT_RETURN("dae")
    CASE_EXT_RETURN("dxf")
    CASE_EXT_RETURN("vda")
    CASE_EXT_RETURN("sxf")
    CASE_EXT_RETURN("stl")
    CASE_EXT_RETURN("xaml")
    CASE_EXT_RETURN("ma")
    CASE_EXT_RETURN("x")
    CASE_EXT_RETURN("step")
    CASE_EXT_RETURN("iges")
    CASE_EXT_RETURN("wrl")
    CASE_EXT_RETURN("ply")
    CASE_EXT_RETURN("c4b")
    CASE_EXT_RETURN("blend")
    CASE_EXT_RETURN("sldprt")
    CASE_EXT_RETURN("3dm")
    CASE_EXT_RETURN("zpr")
    CASE_EXT_RETURN("pmd")
    default: return false;
    }
}

bool ResourceManager::IsTextureFilePath(StringView path) const noexcept
{
    switch (GetHashFromCRC(GetExt(path)))
    {
    CASE_EXT_RETURN("jpg")
    CASE_EXT_RETURN("jpeg")
    CASE_EXT_RETURN("JPG")
    CASE_EXT_RETURN("JPEG")
    CASE_EXT_RETURN("jpe")
    CASE_EXT_RETURN("pjpeg")
    CASE_EXT_RETURN("pjp")
    CASE_EXT_RETURN("png")
    CASE_EXT_RETURN("bmp")
    CASE_EXT_RETURN("tga")
    CASE_EXT_RETURN("dds")
    CASE_EXT_RETURN("gif")
    CASE_EXT_RETURN("tiff")
    CASE_EXT_RETURN("tif")
    CASE_EXT_RETURN("webp")
    CASE_EXT_RETURN("ico")
    CASE_EXT_RETURN("psd")
    CASE_EXT_RETURN("hdr")
    CASE_EXT_RETURN("svg")
    CASE_EXT_RETURN("svgz")
    default: return false;
    }
}

bool ResourceManager::IsAudioFilePath(StringView path) const noexcept
{
    switch (GetHashFromCRC(GetExt(path)))
    {
    CASE_EXT_RETURN("wav")
    CASE_EXT_RETURN("mp3")
    CASE_EXT_RETURN("mp4")
    CASE_EXT_RETURN("wma")
    CASE_EXT_RETURN("asf")
    CASE_EXT_RETURN("3gp")
    CASE_EXT_RETURN("3g2")
    CASE_EXT_RETURN("aac")
    CASE_EXT_RETURN("ogg")
    CASE_EXT_RETURN("oga")
    CASE_EXT_RETURN("mov")
    CASE_EXT_RETURN("m4a")
    CASE_EXT_RETURN("alac")
    CASE_EXT_RETURN("ape")
    CASE_EXT_RETURN("mac")
    CASE_EXT_RETURN("tta")
    CASE_EXT_RETURN("mka")
    CASE_EXT_RETURN("mkv")
    CASE_EXT_RETURN("flac")
    CASE_EXT_RETURN("aiff")
    CASE_EXT_RETURN("aif")
    CASE_EXT_RETURN("aifc")
    default: return false;
    }
}

bool ResourceManager::IsShaderFilePath(StringView path) const noexcept
{
    switch (GetHashFromCRC(GetExt(path)))
    {
    CASE_EXT_RETURN("hlsl")
    CASE_EXT_RETURN("hlsli")
    CASE_EXT_RETURN("cso")
    default: return false;
    }
}

bool ResourceManager::IsProprietaryFilePath(StringView path) const noexcept
{
    switch (GetHashFromCRC(GetExt(path)))
    {
    CASE_EXT_RETURN("model")
    CASE_EXT_RETURN("mesh")
    CASE_EXT_RETURN("material")
    CASE_EXT_RETURN("texture")
    CASE_EXT_RETURN("asset")
    default: return false;
    }
}

String ResourceManager::GetAssetPath(StringView assetName) const noexcept
{
    return ASSET_DATA_DIRECTORY + String(assetName) + ASSET_EXTENSION;
}

String ResourceManager::ToAssetDirectory(StringView resourcePath) const noexcept
{
    // Data/Resource/ : 拡張子の消去
    resourcePath = resourcePath.substr(resourcePath.find("/") + 1, resourcePath.size());
    resourcePath = resourcePath.substr(resourcePath.find("/") + 1, resourcePath.size());
    resourcePath = resourcePath.substr(0, resourcePath.find("."));

    return GetAssetPath(resourcePath);
}