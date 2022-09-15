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

	template<class T>
	T* CatchDragObject() const noexcept;
	IResource* CatchDragObject(uint32_t selectType) const noexcept;

protected:

	DetailsWidget* m_detailsWidget;
};

template<class T>
FORCEINLINE void DetailsObject::ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
	ShowDragDropHelper(T::TypeData.Hash, hovered, r, g, b);
}

template<class T>
FORCEINLINE T* DetailsObject::CatchDragObject() const noexcept
{
	return dynamic_cast<T*>(CatchDragObject(T::TypeData.Hash));
}