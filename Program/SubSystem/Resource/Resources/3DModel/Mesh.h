/**
* @file    Mesh.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IResource.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12VertexBuffer.h"
#include "SubSystem/Resource/ResourceData/ProprietaryMeshData.h"

class Material;

class Mesh : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Mesh, IResource)
public:

	bool Load(StringView path) override;

	/** 主にEditer上で使用される asset や独自データの更新処理 */
	void Update() override;

public:

	/** パラメーター */
	void SetMaterial(Material* material) noexcept;
	Material* GetMaterial() const noexcept;

	D3D12VertexBuffer* GetVertexBuffer() noexcept;
	D3D12IndexBuffer* GetIndexBuffer() noexcept;
	uint32_t GetIndexNum() const noexcept;

private:

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryMeshData m_meshData;

	Material* m_material = nullptr;

	D3D12VertexBuffer m_vertexBuffer;
	D3D12IndexBuffer m_indexBuffer;
};