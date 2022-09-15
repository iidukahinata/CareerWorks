/**
* @file	   EditerHelper.h
* @brief
*
* @date	   2022/09/15 2022îNìxèâî≈
*/
#pragma once


#include "SubSystem/Core/Command/Command.h"
#include "SubSystem/Core/Command/CommandList.h"

class D3D12DescriptorHeap;

class EditorHelper
{
	COMPLETED_DEVELOPMENT()
	EditorHelper() = default;
	COPY_PROHIBITED(EditorHelper);
public:

	static EditorHelper& Get()
	{
		static EditorHelper instance;
		return instance;
	}

public:

	void BegineRenderer() noexcept;

	void Reset() noexcept;

	void AddTexture(D3D12DescriptorHeap* descriptorHeap) noexcept;

public:

	void UndoCommand() noexcept;
	void RedoCommand() noexcept;
	void FlushCommandList() noexcept;

	void AddEditorCommand(UniquePtr<ICommand> command) noexcept;

private:

	CommandList m_commandList;

	Vector<D3D12DescriptorHeap*> m_descriptorHeaps;
};

template<class Lambda, class T>
void RegisterEditorCommand(Lambda&& func, const T& next, const T& pre)
{
	EditorHelper::Get().AddEditorCommand(std::make_unique<EditorCommand<T>>(func, next, pre));
}