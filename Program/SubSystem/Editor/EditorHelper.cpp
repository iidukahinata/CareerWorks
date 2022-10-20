/**
* @file	   EditerHelper.cpp
* @brief
*
* @date	   2022/09/15 2022”N“x‰”Å
*/


#include "EditorHelper.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"
#include "SubSystem/Resource/Resources/3DModel/Model.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Shader.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Resource/Resources/3DModel/Texture.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12GraphicsDevice.h"

void EditorHelper::Initialize(D3D12DescriptorHeap* descriptorHeap) noexcept
{
	m_descriptorHeap = descriptorHeap;

	RegisterIconTexture();
}

void EditorHelper::BegineRenderer() noexcept
{
	// font —p
	constexpr auto offset = 1;

	// reset
	m_numImage = offset;

	m_descriptorHeap->Set();
}

void EditorHelper::Shutdown() noexcept
{
	auto resourceManager = g_context->GetSubsystem<ResourceManager>();

	for (const auto& resource : m_iconTextures)
	{
		const auto& path = resource.second->GetFilePath();

		auto resourceData = resourceManager->GetResourceData(path);
		resourceManager->Unload(resourceData);
	}
}

void EditorHelper::AddImage(void* shaderResourceView, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) noexcept
{
	auto srv = static_cast<D3D12ShaderResourceView*>(shaderResourceView);
	auto texture = srv->GetCPUHandle();

	// V‚µ‚­ texture Data ‚Ì“o˜^
	auto cpuHandle = m_descriptorHeap->GetCPUHandle();
	auto gpuHandle = m_descriptorHeap->GetGPUHandle();

	cpuHandle.ptr += (m_descriptorHeap->GetIncrementSize() * m_numImage);
	gpuHandle.ptr += (m_descriptorHeap->GetIncrementSize() * m_numImage);

	auto size = 1U;
	D3D12GraphicsDevice::Get().GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, &texture, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Image •`‰æ
	ImGui::Image((void*)gpuHandle.ptr, imageSize, uv0, uv1, tint_col, border_col);

	++m_numImage;
}

bool EditorHelper::AddImageButton(void* shaderResourceView, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1, const int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) noexcept
{
	auto srv = static_cast<D3D12ShaderResourceView*>(shaderResourceView);
	auto texture = srv->GetCPUHandle();

	// V‚µ‚­ texture Data ‚Ì“o˜^
	auto cpuHandle = m_descriptorHeap->GetCPUHandle();
	auto gpuHandle = m_descriptorHeap->GetGPUHandle();

	cpuHandle.ptr += (m_descriptorHeap->GetIncrementSize() * m_numImage);
	gpuHandle.ptr += (m_descriptorHeap->GetIncrementSize() * m_numImage);

	auto size = 1U;
	D3D12GraphicsDevice::Get().GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, &texture, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Image •`‰æ
	auto result = ImGui::ImageButton((void*)gpuHandle.ptr, imageSize, uv0, uv1, frame_padding, bg_col, tint_col);

	++m_numImage;

	return result;
}

Texture* EditorHelper::GetIconTexture(IconType type) const noexcept
{
	if (m_iconTextures.contains(type))
	{
		return m_iconTextures.find(type)->second;
	}
	return nullptr;
}

IconType EditorHelper::GetIconTypeFromResourceType(uint32_t type) const noexcept
{
	switch (type)
	{
	case GET_HASH(Scene)	: return Icon_Scene;
	case GET_HASH(Model)	: return Icon_Model;
	case GET_HASH(Mesh)		: return Icon_Mesh;
	case GET_HASH(Material)	: return Icon_Material;
	case GET_HASH(Texture)	: return Icon_Texture;
	case GET_HASH(Shader)	: return Icon_Shader;
	case GET_HASH(AudioClip): return Icon_Audio;
	default					: return Icon_None;
	}
}

uint32_t EditorHelper::GetResourceTypeByIconType(IconType type) const noexcept
{
	switch (type)
	{
	case Icon_Scene	   : return Scene::TypeData.Hash;
	case Icon_Model	   : return Model::TypeData.Hash;
	case Icon_Mesh	   : return Mesh::TypeData.Hash;
	case Icon_Material : return Material::TypeData.Hash;
	case Icon_Texture  : return Texture::TypeData.Hash;
	case Icon_Shader   : return Shader::TypeData.Hash;
	case Icon_Audio    : return AudioClip::TypeData.Hash;
	default			   : return uint32_t(100);
	}
}

void EditorHelper::UndoCommand() noexcept
{
	m_commandList.Undo();
}

void EditorHelper::RedoCommand() noexcept
{
	m_commandList.Redo();
}

void EditorHelper::FlushCommandList() noexcept
{
	m_commandList.FlushCommand();
}

void EditorHelper::AddEditorCommand(UniquePtr<ICommand> command) noexcept
{
	m_commandList.AddCommand(std::move(command));
}

void EditorHelper::RegisterIconTexture() noexcept
{
	auto resourceManager = g_context->GetSubsystem<ResourceManager>();

	Vector<ResourceHandle*> resourceHandles;
	resourceHandles.emplace_back(resourceManager->Load<Texture>("folder_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("scene_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("model_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("mesh_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("material_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("texture_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("shader_icon"));
	resourceHandles.emplace_back(resourceManager->Load<Texture>("audio_icon"));

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	for (int i = 0; i < resourceHandles.size(); ++i)
	{
		const auto& handle = resourceHandles[i];

		handle->WaitForLoadComplete();

		m_iconTextures[i] = handle->GetResource<Texture>();
	}
}