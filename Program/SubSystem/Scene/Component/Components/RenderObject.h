/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/09/02 2022�N�x����
*/
#pragma once


#include "../IComponent.h"

class IResource;
class Model;
class Renderer;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT("�ʃX���b�h�Ŏg�p����Ă���\�������邽�ߏ����^�C�~���O�����ꂷ���@���l���������ǂ�")
	SUB_CLASS(RenderObject)
public:

	void Initialize() override;
	void Remove() override;

	/** �e�`�揈�����L�q�B*/
	virtual void Render() const = 0;

	virtual void SetActive(bool active) override;

protected:

	virtual void Do_Initialize() {}
	virtual void Do_Remove() {}

	void RegisterToRenderer();
	void UnRegisterFromRenderer();

protected:

	Renderer* m_renderer;
};

class ModelRender : public RenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ModelRender)
public:

	void Serialized(FileStream* file) const override;
	void Deserialization(FileStream* file) override;

	void Render() const override;

	void SetModel(IResource* resource) noexcept;
	void SetModel(Model* model) noexcept;
	Model* GetModel() const noexcept;

private:

	Model* m_model = nullptr;
};