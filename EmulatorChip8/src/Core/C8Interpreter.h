#pragma once

#include "Memory/C8Memory.h"

#define INITIAL_PC 0x200

#define PROGRAM_MEM_START 0x200

#define PC_MEM_LOCATION 0x0000
#define PC_SIZE sizeof(c8short)

namespace Chip8
{
	class C8Interpreter
	{
	public:
		C8Interpreter();

		void run();

	private:
		C8Memory memory;
	};
}