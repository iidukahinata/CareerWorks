/**
* @file	   AssetsWidget.h
* @brief
*
* @date	   2022/09/27 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;

class AssetsWidget : public Widget
{
	IN_DEVELOPMENT("���\�[�X����")
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** ���\�[�X�������j���[�o�[�̐��� */
	void ShowResourceMenu() noexcept;

	/** �S���\�[�X�� / �I�����\�[�X����ێ��ȂǍs���B*/
	void ShowResourceList() noexcept;

	/** �I�� Directory �����\�[�X�̗� */
	void ShowCurrentDirectoryResourceList() noexcept;

	/** �t�B���^�[�������Ɏw�薼�̃��\�[�X�̂ݗ� */
	void ShowResourceListByName() noexcept;

	/** ���\�[�X�����Ȃǂ̃��\�b�h���s���B*/
	void ShowResourceHelper() noexcept;

	/** ���\�[�X�������̐ݒ�Ȃǂ̎w����s���B*/
	bool ShowCreateWindow(std::function<void(StringView)> createFunc) noexcept;

	/** ���͒l���� Window �̐؂�ւ��Ȃǂ��s�����߁B*/
	void ChackClickedCommand(IconType type, StringView name) noexcept;

	void DrawThumbnail(IconType type, StringView name) noexcept;

private:

	/** 
	* ���\�[�X�𐶐������ۂɁA�����ł͎g�p���Ȃ����߉��������p�ӁB
	* �R�}���h�����Ɋւ��郊�\�[�X�|�C���^�Ȃǂ����������\�������邪�A
	* Flush ���������L�q���Ă��Ȃ����ߌĂяo�����ōl������K�v������B
	*/
	void UnloadResource(IResource* resource) noexcept;

	/** ���\�[�X�̑I�����s���h���b�O�A���h�h���b�v�Ȃǂ̋@�\�Ɏg�p */
	void ClickResource(IconType type, StringView name) noexcept;

	/** �I�𒆃I�u�W�F�N�g�̓ǂݍ��݂��s���`�ɓK�����������s���܂��B*/
	void DoubleClickResource(IconType type, StringView name) noexcept;

	/** �I�����ꂽ Directory ���\�����̗� */
	void NavigateToDirectory(StringView path) noexcept;

	/** Currnet Directory ������� Directory �ɖ߂� */
	void CurrentDirectoryToParent() noexcept;

	/** Flags */
	bool IsTimeOut() noexcept;

private:

	enum TagType
	{
		TagType_ALL = -1
	};

	// * �V�[���؂�ւ��p
	World* m_world;

	// * Resource ���擾�p
	ResourceManager* m_resourceManager;

	// * �_�u���N���b�N���Ԍv���p
	Stopwatch m_stopwatch;

private:

	// * tag �����Ŋȗ�����}�邽�� (type id ���g�p)
	int m_tag = -1;

	// * filter �����p
	ImGuiTextFilter m_filter;

	bool m_isSelectDirectory;

	// * ���ݕ\�� Directory ��
	String m_currentDirectory;

	// * Assets Directory ����̃t�@�C���\��
	Vector<String> m_directoryTree;

	struct Thumbnail
	{
		String name;
		IconType type;
	};

	// * Current Directory �ɏ������̃��X�g
	Vector<Thumbnail> m_directoryEntries;

	// * ���ݑI�𒆃��\�[�X��
	String m_selectResourceName;

	// * ���ݑI�𒆃��\�[�X
	ResourceData* m_selectResoruce = nullptr;

private:

	// * Helper Window �\���ꏊ
	ImVec2 m_clickedMausePos;

	// * ���\�[�X�������\�b�h
	std::function<IResource*(StringView)> m_resourceCreateFunc;
};