/**
* @file    CommandList.h
* @brief
*
* @date	   2022/09/14 2022年度初版
*/


#include "CommandList.h"
#include "Command.h"

void CommandList::FlushCommand() noexcept
{
	m_commandList.clear();
	m_commandList.shrink_to_fit();

	m_redoCommandList.clear();
	m_redoCommandList.shrink_to_fit();
}

void CommandList::AddCommand(UniquePtr<ICommand> command) noexcept
{
	// redo 用コマンドリスト
	m_redoCommandList.clear();

	command->Execte();

	m_commandList.emplace_back(command.release());
}

void CommandList::Undo() noexcept
{
	if (m_commandList.size() == 0)
	{
		return;
	}

	auto& back = m_commandList.back();

	back->Undo();

	m_redoCommandList.push_back(std::move(back));
	m_commandList.pop_back();
}

void CommandList::Redo() noexcept
{
	if (m_redoCommandList.size() == 0)
	{
		return;
	}

	auto& back = m_redoCommandList.back();

	back->Execte();

	m_commandList.push_back(std::move(back));
	m_redoCommandList.pop_back();
}