#pragma once

#include "pch.h"
#include "Events/C8EventHandler.h"
#include "Events/C8EventType.h"
#include "Input/C8Keymapping.h"

#if PLATFORM_WINDOWS

#define C8_CLOCK_TIMER_ID 1
#define CLOCK_TIME_MS 2500


#define C8_WINDOW_CLASS_NAME "Chip 8 Emulator"
#define C8_L_WINDOW_CLASS_NAME __TEXT("Chip 8 Emulator")

#endif

#define UNSCALED_HEIGHT 128
#define UNSCALED_WIDTH 64

namespace Chip8
{
	class C8Window
	{
		// if true size of window is changed, but set_colors is not called, sometimes the window crashes.

	public:
#if PLATFORM_WINDOWS
		C8Window(const std::wstring& name, int window_width, int window_height, HINSTANCE hInstance, INT nCmdShow);
#endif
		void run();

		~C8Window();

		// colors are set row-wise (0, 0), (1, 0), (2, 0).... (0, 1), (1, 1)....
		// The array will be COPIED, not REFERENCED.
		// The length of the array should be unscaled_height * unscaled_width, that will be scaled automatically.
		void set_colors(c8byte* clrs);

		void set_colors(bool* clrs);

		// keycode here is the actual physical kerboard keycode. Not the chip8 specific one.
		bool is_key_pressed(c8byte keyboard) const;
	
		inline int get_true_height() const { return true_height; }
		inline int get_true_width() const { return true_width; }

		inline int get_unscaled_height() const { return unscaled_height; }
		inline int get_unscaled_width() const { return unscaled_width; }

		void add_window_event_listener(const c8_event_listener& listener);
		void remove_window_event_listener(const c8_event_listener& listener);

		void add_tick_listener(const c8_event_listener& listener);
		void remove_tick_listener(const c8_event_listener& listener);

		// Parameter has two byte: 0th byte: key-state, 1st byte: key-code.
		void add_keyboard_event_listener(const c8_event_listener& listener);
		void remove_keyboard_event_listener(const c8_event_listener& listener);

		void add_update_event_listener(const c8_event_listener& listener);
		void remove_update_event_listener(const c8_event_listener& listener);

		// map will be cloned. old one will be discarded completely.
		void set_key_map(const C8Keymapping* map, int len);

		C8EventHandler tick_timer_handler;

	private:

#if PLATFORM_WINDOWS
		int client_start_x;
		int client_start_y;

		int client_end_x;
		int client_end_y;
#endif

		int unscaled_height;
		int unscaled_width;
		c8Color* unscaled_colors;
		c8Color* scaled_colors;

		int scaled_colors_buffer_size;

		bool modifying_colors;

		int true_height;
		int true_width;

		C8EventHandler window_event_handler;
		C8EventHandler keyboard_event_handler;
		C8EventHandler update_event_handler;

		C8Keymapping* keymap;
		int keymap_length;

		// Returns true if dimensions were successfully upddated.
		bool update_true_dimensions();

		void calculate_scaled_colors(int new_width, int new_height);

		void force_redraw();

		// initializes non platform dependent members.
		void initialise_common_members();

		void invoke_update_event(void* data);

		bool process_messages();

#if PLATFORM_WINDOWS
		HWND hwnd;
		static LRESULT CALLBACK Chip8::C8Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

	};
}