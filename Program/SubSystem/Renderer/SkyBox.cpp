/**
* @file	   SkyBox.cpp
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/


#include "SkyBox.h"
#include "Geometry/Cube.h"
#include "Geometry/Vertex.h"
#include "GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/ICamera.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

void SkyBox::Initialize() noexcept
{
	auto triangleList = Cube::Create<Vertex3D>();
	ASSERT(m_indexBuffer.Create(triangleList.m_indices));
	ASSERT(m_vertexBuffer.Create(triangleList.m_vertices));
	ASSERT(m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix)));
	m_sampler.Create(D3D12_FILTER_MIN_MAG_MIP_LINEAR);
}

void SkyBox::Render(ICamera* mainCamera) noexcept
{
	if (!m_material)
	{
		return;
	}

	// update world matrix
	{
		auto farClip   = mainCamera->GetFar();
		auto transform = mainCamera->GetTransform();

		transform.SetRotation(Math::Quaternion::Identity);
		transform.SetScale(Math::Vector3(farClip - 1.0f));

		const auto world	  = transform.GetWorldMatrix().Transpose().ToMatrixXM();
		const auto view		  = mainCamera->GetViewMatrix().Transpose().ToMatrixXM();
		const auto projection = mainCamera->GetProjectionMatrix().Transpose().ToMatrixXM();

		auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
		buffer->world	   = world;
		buffer->view	   = view;
		buffer->projection = projection;
	}

	// Set ConstantBuffer
	m_constantBufferMatrix.VSSet(0);

	// Set Pipline Objects
	m_material->Render();
	m_sampler.PSSet();

	// Set Mesh Buffer
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void SkyBox::SetMaterial(Material* material) noexcept
{
	m_material = material;
}

Material* SkyBox::GetMaterial() const noexcept
{
	return m_material;
}