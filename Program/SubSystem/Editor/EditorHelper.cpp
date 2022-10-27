/**
* @file	   EditerHelper.cpp
* @brief
*
* @date	   2022/10/21 2022”N“x‰”Å
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
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12DescriptorHeap.h"

void EditorHelper::Initialize(D3D12DescriptorHeap* descriptorHeap, void* finalFrameSRV) noexcept
{
	m_descriptorHeap = descriptorHeap;
	m_finalFrameSRV = finalFrameSRV;

	RegisterIconTexture();
}

void EditorHelper::BegineRenderer() noexcept
{
	// font offset
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
		const auto& path = resource->GetFilePath();
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

	constexpr auto size = 1U;
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

	constexpr auto size = 1U;
	D3D12GraphicsDevice::Get().GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, &texture, nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Image •`‰æ
	auto result = ImGui::ImageButton((void*)gpuHandle.ptr, imageSize, uv0, uv1, frame_padding, bg_col, tint_col);

	++m_numImage;

	return result;
}

void* EditorHelper::GetFinalFrameTexture() const noexcept
{
	return m_finalFrameSRV;
}

Texture* EditorHelper::GetIconTexture(IconType type) const noexcept
{
	ASSERT(type < m_iconTextures.size());
	return m_iconTextures[type];
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

	auto iconTextureLoad = [resourceManager](StringView path) {
		auto handle = resourceManager->Load<Texture>(path);
		handle->WaitForLoadComplete();
		return handle->GetResource<Texture>();
	};

	m_iconTextures.emplace_back(iconTextureLoad("folder_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("scene_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("model_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("mesh_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("material_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("texture_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("shader_icon"));
	m_iconTextures.emplace_back(iconTextureLoad("audio_icon"));
}