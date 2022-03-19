#pragma once

#include "Memory/C8Memory.h"
#include "Window/C8Window.h"
#include "IO/IO.h"

#define INITIAL_PC 0x200

#define PROGRAM_MEM_START 0x200

#define STACK_START 0x2

// bytes
#define STACK_SIZE 32

#define TEXT_SPRITE_MEM_STRART 0x4
#define TEXT_SPRITE_SIZE_BYTES 5
#define TEXT_SPRITE_COUNT 16

#define DEFAULT_SPRITE_WIDTH 8

namespace Chip8
{
	class C8Interpreter
	{

	public:
		C8Interpreter(C8Window& output_window);
		~C8Interpreter();

		void play();
		void pause();
	
		void trigger_key(C8Keystate::C8Keystate keystate, C8Keycode::C8Keycode keycode);
		void trigger_tick();
		void trigger_update();

		bool load_rom(const std::wstring& loc);
		int get_pc();

		c8byte* get_memory_dump();

	private:
		C8Window& output_window;

		C8Memory memory;

		bool* display_buffer;// [UNSCALED_HEIGHT * UNSCALED_WIDTH] ;

		c8byte V[16];
		c8short register_16;

		c8byte delay_register;
		c8byte sound_timer_register;

		int program_counter;
		c8byte stack_pointer;
		// a bit-field.
		c8short keys_pressed;

		bool paused;

		void load_text_sprites();
		void run();

		void clear_screen();

		void update_display();

		void pop_stack();
		void push_stack();

		c8byte get_instruction(c8byte high, c8byte low);
		int get_addr(c8byte high, c8byte low);
		c8byte get_nible(c8byte high, c8byte low);
		c8byte get_x(c8byte high, c8byte low);
		c8byte get_y(c8byte high, c8byte low);
		c8byte get_kk(c8byte high, c8byte low);

		void set_pc(int value);

		void skip_next_instruction();
		void increment_pc();

		bool is_key_pressed(C8Keycode::C8Keycode keycode);

		c8byte generate_random_byte();
	};
}