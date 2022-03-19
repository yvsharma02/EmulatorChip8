#include "pch.h"

#include "C8Window.h"

#if PLATFORM_WINDOWS

#include "windows.h"

#endif

Chip8::C8Window* get_window();

namespace Chip8
{

#if PLATFORM_WINDOWS
	
	bool C8Window::process_messages()
	{
		MSG Msg;
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			if (Msg.message == WM_QUIT)
				return false;
		}

		return true;
	}

	void C8Window::run()
	{
		while (process_messages())
		{
			invoke_update_event(nullptr);
		}
	}

	void CALLBACK on_tick(HWND hwnd, UINT message, UINT id, DWORD dwTime)
	{
		get_window()->tick_timer_handler.invoke(C8EventType::CLOCK_TICK, nullptr);
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
			instance->window_event_handler.invoke(C8EventType::WINDOW_RESCALE, nullptr);
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

				BitBlt(hdc, instance->client_start_x, instance->client_start_y, instance->true_width, instance->true_height, src, 0, 0, SRCCOPY);

				DeleteDC(src);
				DeleteObject(bmp);

				EndPaint(hwnd, &ps);
			}
		}
		break;

		case WM_KEYDOWN:
		{
			for (int i = 0; i < instance->keymap_length; i++)
			{
				if (instance->keymap[i].get_actual_key() != (int)wParam)
					continue;

				// 0th byte for key state. 1st for key pressed (c8 key, not actual key).
				c8byte data[2] = { 255, instance->keymap[i].get_mapped_key() };

				if (instance->keymap[i].get_state() == C8Keystate::NONE)
				{
					instance->keymap[i].set_state(C8Keystate::PRESSED);

					data[0] = C8Keystate::PRESSED;
					instance->keyboard_event_handler.invoke(Chip8::C8EventType::KEYBOARD_INPUT, data);
				}
				else if (instance->keymap[i].get_state() == C8Keystate::PRESSED)
				{
					instance->keymap[i].set_state(C8Keystate::CONTINUED_PRESS);

					data[0] = C8Keystate::CONTINUED_PRESS;
					instance->keyboard_event_handler.invoke(Chip8::C8EventType::KEYBOARD_INPUT, data);
				}
			}
		}
		break;

		case WM_KEYUP:
		{
			for (int i = 0; i < instance->keymap_length; i++)
			{
				if (instance->keymap[i].get_actual_key() != (int)wParam)
					continue;

				if (instance->keymap[i].get_state() == C8Keystate::CONTINUED_PRESS || instance->keymap[i].get_state() == C8Keystate::PRESSED)
				{
					instance->keymap[i].set_state(C8Keystate::NONE);

					c8byte data[2] = { C8Keystate::RELEASED, instance->keymap[i].get_mapped_key()};
					instance->keyboard_event_handler.invoke(C8EventType::KEYBOARD_INPUT, data);
				}
			}
		}
		break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);

		}
	}

	C8Window::C8Window(const std::wstring& window_name, int window_width, int window_height, HINSTANCE hInstance, INT nCmdShow)
	{
		initialise_common_members();

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
			hwnd = CreateWindowEx(0, C8_L_WINDOW_CLASS_NAME, window_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height, NULL, NULL, hInstance, NULL);

			if (hwnd == NULL)
			{
				std::string errorMsg = std::to_string(GetLastError());
				MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			}

			SetTimer(hwnd, C8_CLOCK_TIMER_ID, CLOCK_TIME_MS, (TIMERPROC) on_tick);

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
			SetLayeredWindowAttributes(hwnd, NULL, 255, LWA_ALPHA);

			ShowWindow(hwnd, nCmdShow);
		}
	}
	
	bool C8Window::update_true_dimensions()
	{
		RECT rect;

		if (GetClientRect(hwnd, &rect))
		{
			client_start_x = rect.left;
			client_end_x = rect.right;

			client_start_y = rect.top;
			client_end_y = rect.bottom;

			true_width = (client_end_x - client_start_x);
			true_height = (client_end_y - client_start_y);

			calculate_scaled_colors(true_width, true_height);
			return true;
		}

		return false;
	}

	void C8Window::force_redraw()
	{
		InvalidateRect(hwnd, nullptr, true);
	}

	long C8Window::get_current_time()
	{
		LARGE_INTEGER* time = &(LARGE_INTEGER());
		QueryPerformanceCounter(time);

		return time->QuadPart;
	}

#endif

	void C8Window::initialise_common_members()
	{
		unscaled_height = UNSCALED_HEIGHT;
		unscaled_width = UNSCALED_WIDTH;

		window_event_handler = C8EventHandler();
		update_event_handler = C8EventHandler();
		keyboard_event_handler = C8EventHandler();

		scaled_colors_buffer_size = 0;

		unscaled_colors = nullptr;
		scaled_colors = nullptr;
		keymap = nullptr;
	}

	void C8Window::calculate_scaled_colors(int true_width, int true_height)
	{
		if (unscaled_colors == nullptr)
			return;

		modifying_colors = true;

		long int buffer_size_required = (long int)true_width * (long int)true_height;

		if (scaled_colors_buffer_size < buffer_size_required || scaled_colors == nullptr)
		{
			scaled_colors_buffer_size = buffer_size_required;

			if (scaled_colors != nullptr)
				delete[] scaled_colors;

			scaled_colors = new c8Color[buffer_size_required];
		}
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
		force_redraw();
	}


	void C8Window::invoke_update_event(void* data)
	{
		update_event_handler.invoke(Chip8::C8EventType::UPDATE, data);
	}

	void C8Window::set_colors(bool* clrs)
	{
		modifying_colors = true;

		if (this->unscaled_colors == nullptr)
			this->unscaled_colors = new c8Color[unscaled_height * unscaled_width];

		int c = 0;
		for (int i = 0; i < unscaled_height; i++)
		{
			for (int j = 0; j < unscaled_width; j++)
			{
				this->unscaled_colors[c] = (clrs[c] == true) ? 0xFFFFFF : 0;
				c++;
			}
		}
		calculate_scaled_colors(true_width, true_height);

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

	void C8Window::add_update_event_listener(const c8_event_listener& listener)
	{
		update_event_handler.add_listener(listener);
	}

	void C8Window::remove_update_event_listener(const c8_event_listener& listener)
	{
		update_event_handler.remove_listener(listener);
	}

	void C8Window::add_window_event_listener(const c8_event_listener& listener)
	{
		window_event_handler.add_listener(listener);
	}

	void C8Window::remove_window_event_listener(const c8_event_listener& listener)
	{
		window_event_handler.remove_listener(listener);
	}

	void C8Window::add_keyboard_event_listener(const c8_event_listener& listener)
	{
		keyboard_event_handler.add_listener(listener);
	}

	void C8Window::remove_keyboard_event_listener(const c8_event_listener& listener)
	{
		keyboard_event_handler.remove_listener(listener);
	}

	void C8Window::add_tick_listener(const c8_event_listener& listener)
	{
		tick_timer_handler.add_listener(listener);
	}

	void C8Window::remove_tick_listener(const c8_event_listener& listener)
	{
		tick_timer_handler.remove_listener(listener);
	}
	
	void C8Window::set_key_map(const Chip8::C8Keymapping* new_keymap, int length)
	{
		// maybe overwrite instead of re allocating?
		keymap_length = length;
		if (keymap != nullptr)
		{
			delete [] keymap;
			keymap = nullptr;
		}

		char* kmap = new char[sizeof(C8Keymapping) * length];
		keymap = (C8Keymapping*)kmap;

		for (int i = 0; i < length; i++)
			keymap[i] = C8Keymapping(new_keymap[i]);
	}

	C8Window::~C8Window()
	{
		KillTimer(hwnd, C8_CLOCK_TIMER_ID);

		if (unscaled_colors != nullptr)
			delete[] unscaled_colors;

		if (scaled_colors != nullptr)
			delete[] scaled_colors;

		if (keymap != nullptr)
			delete[] keymap;
	}
}