/**
* @file    EditorEvents.h
* @brief
*
* @date	   2022/10/25 2022�N�x����
*/
#pragma once


class DeleteObjectEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(DeleteObjectEvent)
public:

	DeleteObjectEvent(String deleteObject);
	std::any GetData() override;

private:

	// * �ύX��V�[���I�u�W�F�N�g
	String m_deleteObject;
};

enum class EditorState
{
	Run,

	Stop,

	Pause,

	Unpause,
};

class ChangeEditorStateEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ChangeEditorStateEvent)
public:

	ChangeEditorStateEvent(EditorState editorState);
	std::any GetData() override;

private:

	// * �ύX��G�f�B�^�[���
	EditorState m_editorState;
};