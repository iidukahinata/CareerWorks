/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/10/28 2022�N�x����
*/
#pragma once


#include "../IComponent.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Renderer;
class IResource;
class Model;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT("�ʃX���b�h�Ŏg�p����Ă���\�������邽�ߏ����^�C�~���O�����ꂷ���@���l���������ǂ�")
		SUB_CLASS(RenderObject)
public:

	virtual void OnInitialize() override;
	virtual void OnRegister() override;
	virtual void OnUnRegister() override;

	/** PreZ Pass �p�����_�����O�֐�*/
	virtual void PreRender() = 0;

	/** ���ۂ̃��f���\���������L�q�B*/
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

	// * �e�s��ێ�
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