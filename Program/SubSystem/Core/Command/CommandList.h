/**
* @file    CommandList.h
* @brief
*
* @date	   2022/09/14 2022�N�x����
*/


class ICommand;

class CommandList
{
public:

	/** �o�^�����S�R�}���h���������܂��B */
	void FlushCommand() noexcept;

	void AddCommand(UniquePtr<ICommand> command) noexcept;

	/** �����߂����� */
	void Undo() noexcept;

	/** ��蒼������ */
	void Redo() noexcept;

private:

	// * ���s���ꂽ Undo �p�R�}���h���X�g
	Deque<UniquePtr<ICommand>> m_commandList;

	// * �����߂����ꂽ Redo �p�R�}���h���X�g
	Deque<UniquePtr<ICommand>> m_redoCommandList;
};