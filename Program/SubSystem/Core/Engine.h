/**
* @file    Engine.h
* @brief
*
* @date	   2022/08/02 2022年度初版
*/
#pragma once


#include <Windows.h>

class Engine
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(Engine)
public:

	Engine() = default;

	/** アプリケーション全体のセットアップを行います。*/
	bool Initialize(HINSTANCE hInstance);

	/** エンジンループを実現します。*/
	long MainLoop();

	/**
	* この関数は JobSystem が解放される前に呼び出す必要があります。
	* 各システムがセットした Job 解放処理のため JobSystem にアクセスするためです。
	*/
	void Shutdown();

private:

	/** 主に、通常のウィンドウとデバックウィンドウを生成します。*/
	bool StartUpScreen(HINSTANCE hInstance) const noexcept;

	/** 登録された各システムのセットアップを行う。*/
	bool InitializeSubsystems() noexcept;

private:

	HINSTANCE m_hInstance;

	UniquePtr<Context> m_context;
};