/**
* @file    MeshRender.cpp
* @brief
*
* @date	   2022/11/28 2022?N?x????
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/IRenderer.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void MeshRender::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	if (m_mesh)
	{
		file->Write(m_mesh->GetFilePath());
	}
	else
	{
		file->Write(String());
	}
}

void MeshRender::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	String meshPath;
	file->Read(&meshPath);

	if (meshPath.empty())
	{
		return;
	}

	if (auto resourceManager = GetContext()->GetSubsystem<ResourceManager>())
	{
		if (auto resourceData = resourceManager->GetResourceData(meshPath))
		{
			auto resource = resourceManager->GetResource(resourceData);
			SetMesh(dynamic_cast<Mesh*>(resource));
		}
	}
}

void MeshRender::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<IRenderer>();
	ASSERT(m_renderer);

	if (IsInRenderingThread())
	{
		m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix));
	}
	else
	{
		RegisterRenderCommand([this] { m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix)); });
	}
}

void MeshRender::OnRegister()
{
	IComponent::OnRegister();

	if (GetActive())
	{
		RegisterToRenderer(this);
	}
}

void MeshRender::OnUnRegister()
{
	IComponent::OnUnRegister();

	if (GetActive())
	{
		UnRegisterFromRenderer(this);
	}
}

void MeshRender::OnRemove()
{
	UnRegisterFromRenderer(this);

	m_renderCommandFance.BegineFrame();

	IComponent::OnRemove();
}

void MeshRender::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);

	if (active)
	{
		RegisterToRenderer(this);
	}
	else
	{
		UnRegisterFromRenderer(this);
	}
}

bool MeshRender::Erasable()
{
	return m_renderCommandFance.IsSingle();
}

void MeshRender::SetMesh(Mesh* mesh) noexcept
{
	m_mesh = mesh;
}

Mesh* MeshRender::GetMesh() const noexcept
{
	return m_mesh;
}

void MeshRender::GetUseResourcePaths(Set<String>& resources)
{
	if (m_mesh)
	{
		resources.emplace(m_mesh->GetFilePath());
	}
}

void MeshRender::PreRender()
{
	// ?`?掞?g?p???? MatrixBuffer ?̍X?V
	auto&& handle = m_constantBufferMatrix.GetCPUData();
	auto&& transform = GetTransform().GetWorldMatrix().ToMatrixXM();

	m_renderer->GetTransformCBuffer()->Bind(handle, transform);
	m_constantBufferMatrix.VSSet(0);
}

Batch MeshRender::GetBatch()
{
	Batch batch;

	if (m_mesh)
	{
		batch.constantBuffer = &m_constantBufferMatrix;
		batch.meshes.emplace_back(m_mesh);
	}

	return batch;
}