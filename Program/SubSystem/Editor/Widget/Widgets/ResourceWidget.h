/**
* @file	   ResourceWidget.h
* @brief
*
* @date	   2022/09/07 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class Input;
class World;
class ResourceManager;

class ResourceWidget : public Widget
{
	IN_DEVELOPMENT("���\�[�X����")
public:

	void Initialize() override;
	void PostInitialize() override;
	void Draw() override;

private:

	/** ���\�[�X�������j���[�o�[�̐��� */
	void ShowResourceMenu() noexcept;

	/** �S���\�[�X�� / �I�����\�[�X����ێ��ȂǍs���B*/
	void ShowResourceList() noexcept;

	/** ���\�[�X�����Ȃǂ̃��\�b�h���s���B*/
	void ShowResourceHelper() noexcept;

	/** ���\�[�X�������̐ݒ�Ȃǂ̎w����s���B*/
	void ShowResourceCreateWindow();

	/** ���͒l���� Window �̐؂�ւ��Ȃǂ��s�����߁B*/
	void ChackClickedCommand() noexcept;

private:

	/** 
	* ���\�[�X�𐶐������ۂɁA�����ł͎g�p���Ȃ����߉����L������p�ӁB
	* �R�}���h�����Ɋւ��郊�\�[�X�|�C���^�Ȃǂ����������\�������邪�A
	* Flush ���������L�q���Ă��Ȃ����ߌĂяo�����ōl������K�v������B
	*/
	void UnloadResource(IResource* resource) noexcept;

	/** ���\�[�X�̑I�����s���h���b�O�A���h�h���b�v�Ȃǂ̋@�\�Ɏg�p */
	void ClickResource(uint32_t type, StringView name) noexcept;

	/** �I�𒆃I�u�W�F�N�g�̓ǂݍ��݂��s���`�ɓK�����������s���܂��B*/
	void DoubleClickResource(uint32_t type, StringView name) noexcept;

	/** ���\�[�X�񋓎��̐F�ݒ��o�^ */
	void RegisterResourceColor() noexcept;

	/** Flags */
	bool IsTimeOut() noexcept;
	bool IsShowAllResourceType() noexcept;
	bool IsShowCreateWindow() noexcept;

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

	// * ���ݑI�𒆃��\�[�X
	ResourceData* m_selectResoruce = nullptr;

	// * tag �����Ŋȗ�����}�邽�� (type id ���g�p)
	int m_tag = -1;

	// * filter �����p
	ImGuiTextFilter m_filter;

	// * Image ���g�p����܂ł̊� Button Color
	Map<uint32_t, ImVec4> m_color;

	// * Create �Ȃǂ�񋟂��� Window �̕\���ݒ�
	bool m_isShowHelperWindow = false;

	// * Helper Window �\���ꏊ
	ImVec2 m_clickedMausePos;

	// * ���\�[�X�������\�b�h
	std::function<IResource*(StringView)> m_resourceCreateFunc;
};