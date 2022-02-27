#include "pch.h"

#include "C8Window.h"

#if PLATFORM_WINDOWS

#include "windows.h"

#endif

#if PLATFORM_WINDOWS

void Chip8::C8Window::run()
{
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);

		DispatchMessage(&Msg);
	}
}

LRESULT CALLBACK Chip8::C8Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
		{
			C8Window* instance = (C8Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (!instance->modifyingColors)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				HBITMAP bmp = CreateBitmap(instance->unscaled_height, instance->unscaled_width, 1, 8 * 4, (void*)instance->colors);
				HDC src = CreateCompatibleDC(hdc);
				SelectObject(src, bmp);

				BitBlt(hdc, 0, 0, instance->unscaled_width, instance->unscaled_height, src, 0, 0, SRCCOPY);

				DeleteDC(hdc);
				DeleteObject(bmp);

				EndPaint(hwnd, &ps);
			}
		}

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);

	}
}

Chip8::C8Window::C8Window(const std::wstring& name, int width, int height, HINSTANCE hInstance, INT nCmdShow)
{
	unscaled_height = height;
	unscaled_width = width;

	LPWNDCLASSEXA existing_wc_info = nullptr;

	if (!GetClassInfoExA(nullptr, C8_WINDOW_CLASS_NAME, existing_wc_info))
	{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = WndProc;
		wc.style = 0;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = C8_L_WINDOW_CLASS_NAME;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		if (!RegisterClassEx(&wc))
		{
			std::string errorMsg = std::to_string(GetLastError());
			MessageBox(NULL, (LPCWSTR) errorMsg.c_str(), L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}
		hwnd = CreateWindowEx(0, C8_L_WINDOW_CLASS_NAME, name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

		if (hwnd == NULL)
		{
			std::string errorMsg = std::to_string(GetLastError());
			MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		}

		colors = nullptr;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
		SetLayeredWindowAttributes(hwnd, NULL, 255, LWA_ALPHA);

		ShowWindow(hwnd, nCmdShow);
	}
}

void Chip8::C8Window::set_colors(c8byte* clrs)
{
	modifyingColors = true;

	if (this->colors == nullptr)
		this->colors = new COLORREF[unscaled_height * unscaled_width];

	int c = 0;

	for (int i = 0; i < unscaled_width; i++)
	{
		for (int j = 0; j < unscaled_height; j++)
		{
			int index = i * unscaled_width + unscaled_height;
			
			COLORREF clr = clrs[index] << 16 | clrs[index] << 8 | clrs[index];
			
			this->colors[c++] = clr;
		}
	}

	modifyingColors = false;
}

Chip8::C8Window::~C8Window()
{
	if (colors != nullptr)
		delete colors;
}

#endif