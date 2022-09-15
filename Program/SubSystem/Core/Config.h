/**
* @file    Config.h
* @brief
*
* @date	   2022/09/07 2022年度初版
*/
#pragma once


/**
* @note サブシステムをユーザー指定で変更する関数実装予定
*/
class Config
{
public:

	/** 各システムクラスの生成を行う。*/
	static void RegisterSubsystemsToContainer() noexcept;

private:

	static void SetUpDefaultSettings() noexcept;
};