/**
* @file    CommandList.h
* @brief
*
* @date	   2022/09/14 2022年度初版
*/


class ICommand;

class CommandList
{
public:

	/** 登録した全コマンドを消去します。 */
	void FlushCommand() noexcept;

	void AddCommand(UniquePtr<ICommand> command) noexcept;

	/** 巻き戻し処理 */
	void Undo() noexcept;

	/** やり直し処理 */
	void Redo() noexcept;

private:

	// * 実行された Undo 用コマンドリスト
	Deque<UniquePtr<ICommand>> m_commandList;

	// * 巻き戻しされた Redo 用コマンドリスト
	Deque<UniquePtr<ICommand>> m_redoCommandList;
};