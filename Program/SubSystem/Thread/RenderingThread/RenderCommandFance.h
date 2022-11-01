/**
* @file    RenderCommandFance.h
* @brief
*
* @date	   2022/11/01 2022�N�x����
*/
#pragma once


/**
* GameThread ���� RenderingCommand �ǐ՗p�N���X
* BegineFrame ���Ăяo���ǐՂ��X�^�[�g���AWaitForSingle�AIsSingle �֐����g�p���āA��Ԃ��擾����B
*/
class RenderCommandFance
{
public:

	RenderCommandFance();
	~RenderCommandFance();

	/** Fance �R�}���h�̔��s */
	void BegineFrame() noexcept;

	/**
	* Fance �R�}���h���S�ď��������܂őҋ@
	* �A�v���P�[�V�����̏I�������Ȃ� RenderingThread �̏I�����}���~������ʂŎg�p
	*/
	void WaitForSingle() const noexcept;

	/**
	* Fance �R�}���h���S�ď������ꂽ���𒲂ׂ�
	* GameThread �ł̍X�V�����A��������ł̕���ȂǂŎg�p����
	*/
	bool IsSingle() const noexcept;

private:

	// * �ǉ�����Ă��� Fance �J�E���g
	std::atomic_uint32_t m_fanceValue;

	// * �҂��Ŏg�p����铯���C�x���g
	HANDLE m_hEvent;
};