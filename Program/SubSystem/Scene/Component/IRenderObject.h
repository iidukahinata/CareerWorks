/**
* @file    IRenderObject.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "IComponent.h"

class Mesh;
class Model;

class IRenderObject : public IComponent
{
	COMPLETED_DEVELOPMENT()
public:

	/** Z PrePass 用レンダリング関数 */
	virtual void PreRender()
	{
		ASSERT(0);
	}

	/** 実際のモデル表示処理を記述。*/
	virtual void Render()
	{
		ASSERT(0);
	}

	bool IsTranslucent()
	{
		ASSERT(0); return {};
	}
};

class IMeshRender : public IRenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IMeshRender, IComponent)
public:

	virtual void SetMesh(Mesh* mesh)
	{
		ASSERT(0);
	}
	virtual Mesh* GetMesh() const
	{
		ASSERT(0); return {};
	}
};

class IModelRender : public IRenderObject
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IModelRender, IComponent)
public:

	virtual void SetModel(Model* model)
	{
		ASSERT(0);
	}
	virtual Model* GetModel() const
	{
		ASSERT(0); return {};
	}
};