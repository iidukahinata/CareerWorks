/**
* @file	   DetailsWidget.h
* @brief
*
* @date	   2022/09/13 2022�N�x����
*/
#pragma once


#include "../DetailsWidget.h"

class IResource;

class DetailsObject
{
public:

	DetailsObject(DetailsWidget* detailsWidget);
	virtual void Draw() = 0;

protected:

	/** �h���b�O�A���h�h���b�v���� UI �֌W�̕\�� */
	template<class T>
	void ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept;
	void ShowDragDropHelper(uint32_t selctType, bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept;

	/** Drag Drop ���ꂽ�I�u�W�F�N�g�擾�� Helper �֐� */
	template<class T>
	T* CatchDragObject() const noexcept;
	GameObject* CatchDragGameObject() const noexcept;
	ResourceData* CatchDragResourceData() const noexcept;
	IResource* CatchDragResourceObject(uint32_t selectType) const noexcept;

	/** ���\�[�X�����p Helper �֐� */
	template<class T>
	ResourceData* ShowSearchResourceHelper() noexcept;
	ResourceData* ShowSearchResourceHelper(uint32_t selectType) noexcept;
	bool OpenResourceHelper(UINT index = 0) noexcept;

	template<class T>
	T* LoadResource(ResourceData* resourceData) const noexcept;
	IResource* LoadResource(ResourceData* resourceData) const noexcept;

protected:

	DetailsWidget* m_detailsWidget;

private:

	// * filter �����p
	ImGuiTextFilter m_searchResourceFilter;
};

template<class T>
FORCEINLINE void DetailsObject::ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
	ShowDragDropHelper(T::TypeData.Hash, hovered, r, g, b);
}

template<class T>
FORCEINLINE T* DetailsObject::CatchDragObject() const noexcept
{
	return dynamic_cast<T*>(CatchDragResourceObject(T::TypeData.Hash));
}

template<class T>
FORCEINLINE ResourceData* DetailsObject::ShowSearchResourceHelper() noexcept
{
	return ShowSearchResourceHelper(T::TypeData.Hash);
}

template<>
FORCEINLINE GameObject* DetailsObject::CatchDragObject() const noexcept
{
	return CatchDragGameObject();
}

template<class T>
FORCEINLINE T* DetailsObject::LoadResource(ResourceData* resourceData) const noexcept
{
	return dynamic_cast<T*>(LoadResource(resourceData));
}