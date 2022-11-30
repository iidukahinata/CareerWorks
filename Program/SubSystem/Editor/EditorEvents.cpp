/**
* @file    EditorEvents.cpp
* @brief
*
* @date	   2022/10/28 2022îNìxèâî≈
*/


#include "EditorEvents.h"

ImportResourceEvent::ImportResourceEvent(String importFilePath) :
	m_importFilePath(importFilePath)
{

}

std::any ImportResourceEvent::GetData()
{
	return m_importFilePath;
}

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

UpdateSceneTreeEvent::UpdateSceneTreeEvent()
{
}

std::any UpdateSceneTreeEvent::GetData()
{
	return std::any();
}
