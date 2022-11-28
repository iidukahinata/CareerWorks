/**
* @file    EditorEvents.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


class ImportResourceEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ImportResourceEvent, IEvent)
public:

	ImportResourceEvent(String importFilePath);
	std::any GetData() override;

private:

	// * �ǂݍ��݃t�@�C���p�X
	String m_importFilePath;
};

class DeleteObjectEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(DeleteObjectEvent, IEvent)
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
	SUB_CLASS(ChangeEditorStateEvent, IEvent)
public:

	ChangeEditorStateEvent(EditorState editorState);
	std::any GetData() override;

private:

	// * �ύX��G�f�B�^�[���
	EditorState m_editorState;
};