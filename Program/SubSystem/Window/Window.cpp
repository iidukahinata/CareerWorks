/**
* @file    Window.cpp
* @brief
*
* @date	   2022/08/02 2022年度初版
*/


#include "Window.h"
#include "Subsystem/Timer/Timer.h"

extern Context* g_context;

/** この前方宣言がないと imgui の入力を使用出来ない */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/** ウィンドウプロシージャ */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_ACTIVATE:
		if (LOWORD(wparam) != WA_INACTIVE)
		{
			if (auto timer = g_context->GetSubsystem<Timer>())
				timer->ResetMeasurement();
		}
		break;

	case WM_EXITSIZEMOVE:
		if (auto timer = g_context->GetSubsystem<Timer>())
			timer->ResetMeasurement();
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

bool Window::CreateWindowClass(HINSTANCE hInstance, int width, int height, StringView title, bool fullScreen) noexcept
{
	// タイトルバーと境界線があるウィンドウ
	unsigned long windowStyles = WS_OVERLAPPEDWINDOW;
	if (fullScreen)
	{
		// フルスクリーン時はタイトルバーを消す
		windowStyles = WS_POPUPWINDOW;
		m_width = GetSystemMetrics(SM_CXSCREEN);
		m_height = GetSystemMetrics(SM_CYSCREEN);
		m_fullscreen = true;
	}
	else
	{
		m_width = width;
		m_height = height;
		m_fullscreen = false;
	}

	WNDCLASSEX w = {};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = "windowClass";
	w.hInstance = hInstance;
	if (!RegisterClassEx(&w))
	{
		LOG_ERROR("RegisterClassEx に失敗しました。");
		return false;
	}

	m_hWnd = CreateWindow(
		w.lpszClassName,
		title.data(),
		windowStyles,
		CW_USEDEFAULT,		// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,		// ウィンドウの左上Ｙ座標
		m_width,
		m_height,
		nullptr,			// 親ウィンドウのハンドル
		nullptr,			// メニューハンドル
		hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);			// 追加パラメータ

	if (m_hWnd == nullptr)
	{
		LOG_ERROR("CreateWindow に失敗しました。");
		return false;
	}

	m_hInstance = hInstance;

	return true;
}

bool Window::Tick()
{
	if (PeekMessage(&m_message, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_message);
		DispatchMessage(&m_message);
	}

	return m_message.message != WM_QUIT;
}

HINSTANCE Window::GetHInstance() const noexcept
{
	return m_hInstance;
}

HWND Window::GetHandle() const noexcept
{
	return m_hWnd;
}

long Window::GetMessage() const noexcept
{
	return m_message.message;
}

int Window::GetWindowWidth() const noexcept
{
	return m_width;
}

int Window::GetWindowHeight() const noexcept
{
	return m_height;
}

bool Window::IsFullscreen() const noexcept
{
	return m_fullscreen;
}