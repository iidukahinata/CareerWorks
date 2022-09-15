/**
 * @file	ISubsystem.h
 * @brief
 *
 * @date	2022/08/02 2022îNìxèâî≈
 */
#pragma once


class Context;

class ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUPER_CLASS(ISubsystem)
	COPY_PROHIBITED(ISubsystem)
public:

	ISubsystem() = default;
	virtual ~ISubsystem() {}

	virtual bool Initialize() { return true; }
	virtual void Shutdown() {}

	Context* GetContext() const noexcept;
};