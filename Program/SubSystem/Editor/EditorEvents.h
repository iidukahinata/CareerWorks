/**
* @file    EditorEvents.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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

	// * 読み込みファイルパス
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

	// * 変更先シーンオブジェクト
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

	// * 変更後エディター状態
	EditorState m_editorState;
};