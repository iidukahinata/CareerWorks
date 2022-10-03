/**
* @file    RenderObject.h
* @brief
*
* @date	   2022/10/03 2022年度初版
*/
#pragma once


#include "../IComponent.h"

class Renderer;
class IResource;
class Model;

class RenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT("別スレッドで使用されている可能性があるため消去タイミングをずれす方法を考えた方が良い")
	SUB_CLASS(RenderObject)
public:

	virtual void OnInitialize() override;
	virtual void OnStart() override;
	virtual void OnStop() override;

	/** 各描画処理を記述。*/
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