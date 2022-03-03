#pragma once

#include "pch.h"
#include "Events/C8EventHandler.h"
#include "Events/C8EventType.h"

#if PLATFORM_WINDOWS

#define C8_WINDOW_CLASS_NAME "Chip 8 Emulator"
#define C8_L_WINDOW_CLASS_NAME __TEXT("Chip 8 Emulator")

#endif

namespace Chip8
{
	class C8Window
	{
		// if true size of window is changed, but set_colors is not called, sometimes the window crashes.

	public:
#if PLATFORM_WINDOWS
		C8Window(const std::wstring& name, int width, int height, HINSTANCE hInstance, INT nCmdShow);

		void run_for_windows();
#endif
		// Color is an byte because c8 window is grayscale anyway.
		void set_pixel_color(int x, int y, c8byte color);

		void clear();

		~C8Window();

		// colors are set row-wise (0, 0), (1, 0), (2, 0).... (0, 1), (1, 1)....
		// The array will be COPIED, not REFERENCED.
		// The length of the array should be unscaled_height * unscaled_width, that will be scaled automatically.
		void set_colors(c8byte* clrs);

		// keycode here is the actual physical kerboard keycode. Not the chip8 specific one.
		bool is_key_pressed(c8byte keyboard) const;
	
		inline int get_true_height() const { return true_height; }
		inline int get_true_width() const { return true_width; }

		inline int get_unscaled_height() const { return unscaled_height; }
		inline int get_unscaled_width() const { return unscaled_width; }

		void add_window_event_listener(const c8_event_listener& listener);

		void remove_window_event_listener(const c8_event_listener& listener);

	private:
		int unscaled_height;
		int unscaled_width;
		c8Color* unscaled_colors;
		c8Color* scaled_colors;
		bool modifying_colors;

		int true_height;
		int true_width;

		C8EventHandler window_event_handler;

		// Returns true if dimensions were successfully upddated.
		bool update_true_dimensions();

		void calculate_scaled_colors(int new_width, int new_height);

#if PLATFORM_WINDOWS
		HWND hwnd;
		static LRESULT CALLBACK Chip8::C8Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

	};
}