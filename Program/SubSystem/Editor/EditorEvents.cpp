/**
* @file    EditorEvents.cpp
* @brief
*
* @date	   2022/10/25 2022îNìxèâî≈
*/


#include "EditorEvents.h"

DeleteObjectEvent::DeleteObjectEvent(String deleteObject) : 
	m_deleteObject(deleteObject)
{

}

std::any DeleteObjectEvent::GetData()
{
	return m_deleteObject;
}

ChangeEditorStateEvent::ChangeEditorStateEvent(EditorState editorState) :
	m_editorState(editorState)
{

}

std::any ChangeEditorStateEvent::GetData()
{
	return m_editorState;
}