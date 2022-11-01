/**
* @file    Mesh.h
* @brief
*
* @date	   2022/10/30 2022�N�x����
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
	SUB_CLASS(Mesh)
public:

	bool Load(StringView path) override;

	/** ���Editer��Ŏg�p����� asset ��Ǝ��f�[�^�̍X�V���� */
	void Update() override;

	void PreRender() noexcept;
	void Render() noexcept;

	/** �p�����[�^�[ */
	void SetMaterial(Material* material) noexcept;
	Material* GetMaterial() const noexcept;

private:

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryMeshData m_meshData;

	Material* m_material = nullptr;

	D3D12VertexBuffer m_vertexBuffer;
	D3D12IndexBuffer m_indexBuffer;
};