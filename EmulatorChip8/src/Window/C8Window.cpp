#include "pch.h"

#include "C8Window.h"

#if PLATFORM_WINDOWS

#include "windows.h"

#endif

namespace Chip8
{

#if PLATFORM_WINDOWS
	
	void C8Window::run_for_windows()
	{
		MSG Msg;
		while (GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);

			DispatchMessage(&Msg);
		}
	}

	LRESULT CALLBACK C8Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		C8Window* instance = (C8Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			instance->update_true_dimensions();
			instance->window_event_handler.invoke(C8EventType::WINDOW_RESCALE);
			break;

		case WM_PAINT:
		{
			if (!instance->modifying_colors && instance->scaled_colors != nullptr)
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				
				HBITMAP bmp = CreateBitmap(instance->true_width, instance->true_height, 1, 8 * 4, (void*)instance->scaled_colors);
				HDC src = CreateCompatibleDC(hdc);
				SelectObject(src, bmp);

				BitBlt(hdc, 0, 0, instance->true_width, instance->true_height, src, 0, 0, SRCCOPY);

				DeleteDC(hdc);
				DeleteObject(bmp);

				EndPaint(hwnd, &ps);
			}
		}

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);

		}
	}

	C8Window::C8Window(const std::wstring& name, int width, int height, HINSTANCE hInstance, INT nCmdShow)
	{
		unscaled_height = height;
		unscaled_width = width;

		window_event_handler = C8EventHandler();

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
				MessageBox(NULL, (LPCWSTR)errorMsg.c_str(), L"Error!", MB_ICONEXCLAMATION | MB_OK);
			}
			hwnd = CreateWindowEx(0, C8_L_WINDOW_CLASS_NAME, name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

			if (hwnd == NULL)
			{
				std::string errorMsg = std::to_string(GetLastError());
				MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			}

			unscaled_colors = nullptr;
			scaled_colors = nullptr;

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
			SetLayeredWindowAttributes(hwnd, NULL, 255, LWA_ALPHA);

			ShowWindow(hwnd, nCmdShow);
		}
	}


	bool C8Window::update_true_dimensions()
	{
		RECT rect;

		if (GetWindowRect(hwnd, &rect))
		{
			true_width = rect.right - rect.left;
			true_height  = rect.bottom - rect.top;

			calculate_scaled_colors(true_width, true_height);
			InvalidateRect(hwnd, nullptr, true);
			return true;
		}

		return false;
	}

#endif

	void C8Window::calculate_scaled_colors(int true_width, int true_height)
	{
		if (unscaled_colors == nullptr)
			return;

		modifying_colors = true;

		if (scaled_colors != nullptr)
			delete[] scaled_colors;

		scaled_colors = new c8Color[(long int)true_width * (long int)true_height];
		long int c = 0;

		for (int i = 0; i < true_height; i++)
		{
			for (int j = 0; j < true_width; j++)
			{
				float xRatio = (float)j / (true_width - 1);
				float yRatio = (float)i / (true_height - 1);

				int unscaled_x = (int)((unscaled_width - 1) * xRatio);
				int unscaled_y = (int)((unscaled_height - 1) * yRatio);

				int unscaled_index = unscaled_y * unscaled_width + unscaled_x;

				scaled_colors[c++] = unscaled_colors[unscaled_index];
			}
		}

		modifying_colors = false;
	}

	void C8Window::set_colors(c8byte* clrs)
	{
		modifying_colors = true;

		if (this->unscaled_colors == nullptr)
			this->unscaled_colors = new c8Color[unscaled_height * unscaled_width];

		int c = 0;

		for (int i = 0; i < unscaled_height; i++)
		{
			for (int j = 0; j < unscaled_width; j++)
			{
				c8Color clr = clrs[c] << 16 | clrs[c] << 8 | clrs[c];

				this->unscaled_colors[c++] = clr;
			}
		}

		calculate_scaled_colors(true_width, true_height);

		modifying_colors = false;
	}

	void C8Window::add_window_event_listener(const c8_event_listener& listener)
	{
		window_event_handler.add_listener(listener);
	}

	void C8Window::remove_window_event_listener(const c8_event_listener& listener)
	{
		window_event_handler.remove_listener(listener);
	}

	C8Window::~C8Window()
	{
		if (unscaled_colors != nullptr)
			delete[] unscaled_colors;

		if (scaled_colors != nullptr)
			delete[] scaled_colors;
	}
}