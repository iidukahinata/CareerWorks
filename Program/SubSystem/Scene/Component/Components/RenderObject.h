/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/10/28 2022年度初版
*/
#pragma once


#include "../IComponent.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Renderer;
class IResource;
class Model;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT("別スレッドで使用されている可能性があるため消去タイミングをずれす方法を考えた方が良い")
		SUB_CLASS(RenderObject)
public:

	virtual void OnInitialize() override;
	virtual void OnRegister() override;
	virtual void OnUnRegister() override;

	/** PreZ Pass 用レンダリング関数*/
	virtual void PreRender() = 0;

	/** 実際のモデル表示処理を記述。*/
	virtual void Render() = 0;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

protected:

	Renderer* m_renderer = nullptr;

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

	void SetModel(IResource* resource) noexcept;
	void SetModel(Model* model) noexcept;
	Model* GetModel() const noexcept;

private:

	Model* m_model = nullptr;
};