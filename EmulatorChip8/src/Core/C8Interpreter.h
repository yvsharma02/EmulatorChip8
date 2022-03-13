#pragma once

#include "Memory/C8Memory.h"
#include "Window/C8Window.h"

#define INITIAL_PC 0x200

#define PROGRAM_MEM_START 0x200

#define TEXT_SPRITE_MEM_STRART 0x2
#define TEXT_SPRITE_SIZE_BYTES 5
#define TEXT_SPRITE_COUNT 16

namespace Chip8
{
	class C8Interpreter
	{


	public:
		C8Interpreter();

		void run();

	private:
		C8Memory memory;

		c8byte* registers_8[16];
		c8short stack[8];
		c8short register_16;

		c8byte delay_register;
		c8byte sound_timer_register;

		c8short program_counter;
		c8byte stack_pointer;

//		C8Window window;

		void load_text_sprites();
	};
}