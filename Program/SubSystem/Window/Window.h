/**
* @file    Window.h
* @brief
*
* @date	   2022/09/06 2022年度初版
*/
#pragma once


#include <Windows.h>

class Window
{
	COMPLETED_DEVELOPMENT()
	Window() = default;
	COPY_PROHIBITED(Window)
public:

	static Window& Get() noexcept
	{
		static Window instance;
		return instance;
	}

public:

	/**
	* 設定された値で Window を作成
	* 
	* @param hInstance [入力] 実行時のインスタンスハンドルを指定する必要があります。
	* @param width [入力] ０以下の値を指定しないでください。
	* @param height [入力] ０以下の値を指定しないでください。
	* @param title [入力] 生成する Window のタイトルバーに出力する文字列。nullは考慮していない。
	* @param fullScreen [入力] trueが指定された時は hInstance 以外の引数値の意味はなくなります。
	*/
	bool CreateWindowClass(HINSTANCE hInstance, int width, int height, StringView title, bool fullScreen) noexcept;

	/**
	* アプリケーションに関するメッセージを取得し、
	* window message から window 状態を調べる
	* 
	* @return false の場合は window の終了合図
	*/
	bool Tick();

public:

	/** 最後に更新された時の window 状態の詳細情報を返します。*/
	long GetMessage() const noexcept;

	/** 生成時に指定された実行時インスタンスハンドルを返す。*/
	HINSTANCE GetHInstance() const noexcept;

	/** Create で生成された Window ハンドルオブジェクトを返す。*/
	HWND GetHandle() const noexcept;

	/** window size */
	int	 GetWindowWidth() const noexcept;
	int	 GetWindowHeight() const noexcept;
	bool IsFullscreen() const noexcept;

private:

	// * 実行時インスタンスハンドル
	HINSTANCE m_hInstance;

	// * 生成した window 識別子
	HWND m_hWnd;

	//*  window 状態を外部から取得するための変数
	MSG m_message;

	// * window size
	int m_width;
	int m_height;
	bool m_fullscreen;
};