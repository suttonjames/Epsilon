#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "language_layer.h"

static b32 is_running = true;

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = -1;

	switch (message) {
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
	{
		is_running = false;
		PostQuitMessage(0);
	} break;

	default:
		result = DefWindowProc(window, message, wParam, lParam);
	}
	return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSA window_class = { 0 };

	window_class.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	window_class.lpfnWndProc = WindowProc;
	window_class.hInstance = hInstance;
	window_class.lpszClassName = "Win32Window";

	RegisterClassA(&window_class);

	HWND window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Engine",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0);

	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	HDC device_context = GetDC(window);

	while (is_running) {
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}