/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/09/02 2022年度初版
*/
#pragma once


#include "../IComponent.h"

class IResource;
class Model;
class Renderer;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT("別スレッドで使用されている可能性があるため消去タイミングをずれす方法を考えた方が良い")
	SUB_CLASS(RenderObject)
public:

	void Initialize() override;
	void Remove() override;

	/** 各描画処理を記述。*/
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