#pragma once

#include "pch.h"

#define C8_MEMORY_SIZE 4096

namespace Chip8
{
	class C8Memory
	{
	private:
		int dataSize;
		c8byte* data;

	public:
		C8Memory();
		~C8Memory();

		int size() const;

		c8byte& operator [](int address);

		void get_stack_byte()
		{

		}
	};
}