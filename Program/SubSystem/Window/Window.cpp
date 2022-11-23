/**
* @file    Window.cpp
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/


#include "Window.h"
#include "Subsystem/Timer/Timer.h"
#include "SubSystem/Script/ScriptEngine.h"

/** ���̑O���錾���Ȃ��� imgui �̓��͂��g�p�o���Ȃ� */
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/** �E�B���h�E�v���V�[�W�� */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

	switch (msg)
	{
	case WM_DROPFILES:
	{
		auto hDropInfo = reinterpret_cast<HDROP>(wparam);
		char buffer[MAX_PATH + 1];

		DragQueryFile(hDropInfo, 0, buffer, MAX_PATH + 1);

		NotifyEvent<ImportResourceEvent>(buffer);

		DragFinish(hDropInfo);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wparam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;
	}
	case WM_ACTIVATE:
	{
		if (LOWORD(wparam) != WA_INACTIVE)
		{
			if (auto timer = g_context->GetSubsystem<Timer>())
			{
				timer->ResetMeasurement();
			}
#ifdef IS_EDITOR
			if (auto script = g_context->GetSubsystem<ScriptEngine>())
			{
				script->RebuildAllScript();
			}
#endif // IS_EDITOR
		}
		break;
	}
	case WM_EXITSIZEMOVE: 
	{
		if (auto timer = g_context->GetSubsystem<Timer>())
		{
			timer->ResetMeasurement();
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

bool Window::CreateWindowClass(HINSTANCE hInstance, int width, int height, StringView title, bool fullScreen) noexcept
{
	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
	unsigned long windowStyles = WS_SIZEBOX | WS_OVERLAPPEDWINDOW;
	if (fullScreen)
	{
		// �t���X�N���[�����̓^�C�g���o�[������
		windowStyles = WS_POPUPWINDOW;
		m_width  = GetSystemMetrics(SM_CXSCREEN);
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
	w.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
	w.hInstance = hInstance;
	if (!RegisterClassEx(&w))
	{
		LOG_ERROR("RegisterClassEx �Ɏ��s���܂����B");
		return false;
	}

	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRectEx(&wr, WS_CAPTION | WS_MINIMIZE | WS_SYSMENU, FALSE, NULL);

	m_hWnd = CreateWindow(
		w.lpszClassName,
		title.data(),
		windowStyles,
		CW_USEDEFAULT,		// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,		// �E�B���h�E�̍���x���W
		m_width,
		m_height,
		nullptr,			// �e�E�B���h�E�̃n���h��
		nullptr,			// ���j���[�n���h��
		hInstance,			// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);			// �ǉ��p�����[�^

	if (m_hWnd == nullptr)
	{
		LOG_ERROR("CreateWindow �Ɏ��s���܂����B");
		return false;
	}

	m_hInstance = hInstance;

#ifdef IS_EDITOR
	DragAcceptFiles(m_hWnd, true);

	SetUpListenerObjects();
#endif // IS_EDITOR

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

void Window::Shutdown()
{
	UnregisterClass("windowClass", m_hInstance);
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

#ifdef IS_EDITOR
void Window::SetUpListenerObjects() noexcept
{
	m_destroyWindowListener.SetFunction([this](std::any data) {

		DestroyWindow(Window::Get().GetHandle());

	});

	m_commandWindowListener.SetFunction([](std::any data) {

		int cmd = std::any_cast<int>(data);
		switch (cmd)
		{
		case SW_MINIMIZE:        

			ShowWindow(Window::Get().GetHandle(), SW_MINIMIZE);
			CloseWindow(Window::Get().GetHandle());
			break;

		case SW_MAXIMIZE:

			ShowWindow(Window::Get().GetHandle(), SW_MAXIMIZE);
			break;

		default: 
			break;
		}

	});

	m_destroyWindowListener.RegisterToEventManager<DestroyWindowEvent>();
	m_commandWindowListener.RegisterToEventManager<CommandWindowEvent>();
}
#endif // IS_EDITOR