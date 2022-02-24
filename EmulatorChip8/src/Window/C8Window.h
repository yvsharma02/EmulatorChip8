#pragma once

#include "pch.h"

#if PLATFORM_WINDOWS

#define C8_WINDOW_CLASS_NAME "Chip8Window"
#define C8_L_WINDOW_CLASS_NAME __TEXT("Chip8Window")

#endif

namespace Chip8
{
	class C8Window
	{
	public:
#if PLATFORM_WINDOWS
		C8Window(const std::wstring& name, int width, int height, HINSTANCE hInstance, INT nCmdShow);

		void run();
#endif
		// Color is an byte because c8 window is grayscale anyway.
		void set_pixel_color(int x, int y, c8byte color);

		void clear();

		// colors are set row-wise (0, 0), (1, 0), (2, 0).... (0, 1), (1, 1)....
		void set_colors(c8byte* colors);

		// keycode here is the actual physical kerboard keycode. Not the chip8 specific one.
		bool is_key_pressed(c8byte keyboard);
	
	private:
		int unscaled_height;
		int unscaled_width;
#if PLATFORM_WINDOWS
		HWND hwnd;
		static LRESULT CALLBACK Chip8::C8Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

	};
}