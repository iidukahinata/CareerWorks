/**
* @file	   DragDrop.cpp
* @brief
*
* @date	   2022/09/09 2022îNìxèâî≈
*/


#include "DragDrop.h"

void DragDrop::StartDrag(DragDropType type, std::any dragObject) noexcept
{
    m_dragObject.first = type;
    m_dragObject.second = dragObject;
}

void DragDrop::EndDrag() noexcept
{
    m_dragObject.first = DragDrop_Unknown;
    m_dragObject.second = nullptr;
}

std::any DragDrop::GetDragObject() noexcept
{
    return m_dragObject.second;
}

bool DragDrop::HasDragObject() const noexcept
{
    return m_dragObject.first != DragDrop_Unknown;
}

bool DragDrop::HasGameObject() const noexcept
{
    return m_dragObject.first == DragDrop_GameObject;
}

bool DragDrop::HasResource() const noexcept
{
    return m_dragObject.first == DragDrop_Resource;
}