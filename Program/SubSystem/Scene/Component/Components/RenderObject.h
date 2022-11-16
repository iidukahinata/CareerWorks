/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/11/04 2022年度初版
*/
#pragma once


#include "../IComponent.h"
#include "SubSystem/Thread/RenderingThread/RenderCommandFance.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Renderer;
class IResource;
class Model;
class Mesh;
class Material;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(RenderObject)
public:

	virtual ~RenderObject() = default;

	virtual void OnInitialize() override;
	virtual void OnRegister() override;
	virtual void OnUnRegister() override;
	virtual void OnRemove() override;

	void SetActive(bool active) override;

	bool Erasable() override;

	/** Z PrePass 用レンダリング関数 */
	virtual void PreRender() = 0;

	/** 実際のモデル表示処理を記述。*/
	virtual void Render() = 0;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

protected:

	Renderer* m_renderer = nullptr;

	bool m_isRegister = false;

	RenderCommandFance m_renderCommandFance;

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

class ModelRender : public RenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ModelRender)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void PreRender() override;
	void Render() override;

public:

	void SetModel(Model* model) noexcept;
	Model* GetModel() const noexcept;

private:

	Model* m_model = nullptr;
};

class MeshRender : public RenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(MeshRender)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void PreRender() override;
	void Render() override;

public:

	void SetMesh(Mesh* mesh) noexcept;
	Mesh* GetMesh() const noexcept;

private:

	Mesh* m_mesh = nullptr;
};