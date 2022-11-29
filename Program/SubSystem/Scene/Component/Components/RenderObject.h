/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IRenderObject.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class IRenderer;
class IResource;
class Model;
class Mesh;
class Material;

class RenderObject
{
	COMPLETED_DEVELOPMENT()
protected:

	void RegisterToRenderer(IRenderObject* renderObject) noexcept;
	void UnRegisterFromRenderer(IRenderObject* renderObject) noexcept;

protected:

	RenderCommandFance m_renderCommandFance;

	IRenderer* m_renderer = nullptr;

	bool m_isRegister = false;

	struct ConstantBufferMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldView;
		DirectX::XMMATRIX worldViewProjection;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX viewProjection;
		DirectX::XMMATRIX viewProjectionInverse;
		DirectX::XMMATRIX projection;
	};

	// * 各行列保持
	D3D12ConstantBuffer m_constantBufferMatrix;
};

class ModelRender : public IModelRender, RenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ModelRender, IModelRender)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	void SetActive(bool active) override;
	bool Erasable() override;

public:

	void PreRender() override;
	void Render() override;

public:

	void SetModel(Model* model) noexcept;
	Model* GetModel() const noexcept;

public:

	void GetUseResourcePaths(Vector<String>& resources) override;

private:

	Model* m_model = nullptr;
};

class MeshRender : public IMeshRender, RenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(MeshRender, IMeshRender)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;
	void OnRemove() override;

	void SetActive(bool active) override;
	bool Erasable() override;

public:

	void PreRender() override;
	void Render() override;

public:

	void SetMesh(Mesh* mesh) noexcept;
	Mesh* GetMesh() const noexcept;

public:

	void GetUseResourcePaths(Vector<String>& resources) override;

private:

	Mesh* m_mesh = nullptr;
};