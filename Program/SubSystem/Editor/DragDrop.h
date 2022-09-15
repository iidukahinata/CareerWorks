/**
* @file	   DragDrop.h
* @brief
*
* @date	   2022/09/09 2022îNìxèâî≈
*/
#pragma once


enum DragDropType
{
	DragDrop_Unknown,
	DragDrop_GameObject,
	DragDrop_Resource,
};

class DragDrop
{
	IN_DEVELOPMENT()
public:

	static DragDrop& Get()
	{
		static DragDrop instance;
		return instance;
	}

	void StartDrag(DragDropType type, std::any dragObject) noexcept;
	void EndDrag() noexcept;

	std::any GetDragObject() noexcept;

	bool HasDragObject() const noexcept;
	bool HasGameObject() const noexcept;
	bool HasResource() const noexcept;

private:

	std::pair<DragDropType, std::any> m_dragObject;
};