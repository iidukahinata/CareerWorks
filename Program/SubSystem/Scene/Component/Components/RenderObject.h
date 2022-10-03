/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


#include "../IComponent.h"

class Renderer;
class IResource;
class Model;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT("�ʃX���b�h�Ŏg�p����Ă���\�������邽�ߏ����^�C�~���O�����ꂷ���@���l���������ǂ�")
	SUB_CLASS(RenderObject)
public:

	virtual void OnInitialize() override;
	virtual void OnStart() override;
	virtual void OnStop() override;

	/** �e�`�揈�����L�q�B*/
	virtual void Render() const = 0;

private:

	void RegisterToRenderer() noexcept;
	void UnRegisterFromRenderer() noexcept;

protected:

	Renderer* m_renderer = nullptr;
};

class ModelRender : public RenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ModelRender)
public:

	void Serialized(FileStream* file) const override;
	void Deserialization(FileStream* file) override;

	void Render() const override;

public:

	void SetModel(IResource* resource) noexcept;
	void SetModel(Model* model) noexcept;
	Model* GetModel() const noexcept;

private:

	Model* m_model = nullptr;
};