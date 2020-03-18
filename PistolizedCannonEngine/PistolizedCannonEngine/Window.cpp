#include "Window.h"

Window* window = nullptr;

Window::Window()
{
}

LRESULT CALLBACK Wndproc(HWND hwnd, UINT msg,  WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//	Event fired when the window will be created.
		window->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		//	Event fired when the window will be destroyed.
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:	
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	//	setting wc's attributes
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &Wndproc;

	if (!::RegisterClassEx(&wc))	//	passing the address of 'wc' object to RegisterClassEx
	{								//	if the registration of the class fails, it will return false.
		return false;
	}
	
	if (!window)
	{
		window = this;
	}

	//	creation of the window
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"PistolizedCannon Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, 
						NULL, NULL, NULL, NULL);
		
	//	if creation of window fails, return false
	if (!m_hwnd)
	{
		return false;
	}

	//	Show the window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);	

	//	set this flag to 'true' to indicate that the window is intialized and running
	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	window->onUpdate();

	Sleep(0);

	return true;
}

bool Window::release()
{
	//	Destroy Window
	if (!::DestroyWindow(m_hwnd))
	{
		return false;
	}
	return true;
}

bool Window::isRunning()
{
	return m_is_run;
}

void Window::onDestroy()
{
	m_is_run = false;
}

Window::~Window()
{
}
