/**
* @file	   SkyBox.cpp
* @brief
*
* @date	   2022/10/28 2022年度初版
*/


#include "SkyBox.h"
#include "Renderer.h"
#include "Geometry/Cube.h"
#include "Geometry/Vertex.h"
#include "GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Resource/Resources/3DModel/Texture.h"

void SkyBox::Initialize() noexcept
{
	ASSERT(CreateRootSinature());
	ASSERT(CreatePipeline());

	auto triangleList = Cube::Create<Vertex3D>();
	ASSERT(m_indexBuffer.Create(triangleList.m_indices));
	ASSERT(m_vertexBuffer.Create(triangleList.m_vertices));

	ASSERT(m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix)));
	m_sampler.Create(D3D12_FILTER_MIN_MAG_MIP_LINEAR);
}

void SkyBox::Render(Camera* mainCamera) noexcept
{
	if (!m_texture)
	{
		return;
	}

	// update world matrix
	{
		auto farClip   = mainCamera->GetFar();
		auto transform = mainCamera->GetTransform();

		transform.SetRotation(Math::Vector3::Zero);
		transform.SetScale(Math::Vector3(farClip - 1.0f));

		const auto world	  = transform.GetWorldMatrix().Transpose().ToMatrixXM();
		const auto view		  = mainCamera->GetViewMatrix().Transpose().ToMatrixXM();
		const auto projection = mainCamera->GetProjectionMatrix().Transpose().ToMatrixXM();

		auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
		buffer->world	   = world;
		buffer->view	   = view;
		buffer->projection = projection;
	}

	// Set PipelineState
	m_pipeline.Set();

	// Set ConstantBuffer
	m_constantBufferMatrix.VSSet(0);

	// Set Sampler
	m_sampler.PSSet(0);

	// Set Texture
	m_texture->PSSet(0);

	// Set Mesh Buffer
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void SkyBox::SetTexture(Texture* texture) noexcept
{
	m_texture = texture;
}

Texture* SkyBox::GetTexture() const noexcept
{
	return m_texture;
}

bool SkyBox::CreateRootSinature() noexcept
{
	Array<CD3DX12_DESCRIPTOR_RANGE1, 3> descTblRanges = {};
	Array<CD3DX12_ROOT_PARAMETER1, 3> rootParams = {};

	//===定数バッファ用設定=====================================
	descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
	//==========================================================

	//===テクスチャ用設定=======================================
	descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 16, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[1].InitAsDescriptorTable(1, &descTblRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	//==========================================================

	//===Sampler用設定==========================================
	descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[2].InitAsDescriptorTable(1, &descTblRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	//==========================================================

	return m_rootSignature.Create(rootParams.size(), rootParams.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
}

bool SkyBox::CreatePipeline() noexcept
{
	constexpr auto SKYBOX_SHADER_PATH = "Data/Resource/Shader/SkyBox.hlsl";

	ASSERT(m_vertexShader.Compile(SKYBOX_SHADER_PATH, VertexShader));
	ASSERT(m_pixelShader.Compile(SKYBOX_SHADER_PATH, PixelShader));

	GraphicsPipelineStateDesc desc;
	desc.VS				  = &m_vertexShader;
	desc.PS				  = &m_pixelShader;
	desc.BlendMode		  = BLEND_MODE::BLEND_MODE_ALPHA;
	desc.RasterizerState  = RASTERIZER_STATE::NO_CULL;
	desc.PrimitiveType	  = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;

	return m_pipeline.Create(desc, &m_rootSignature);
}