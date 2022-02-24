#pragma once

#define C8_MEMORY_SIZE 4096

namespace Chip8
{
	class Memory
	{
	private:
		int dataSize;
		c8byte* data;

	public:
		Memory();
		~Memory();
//		c8byte getByte(int address) const;
//		void setByte(int address);

		int size() const;

		c8byte& operator [](int address);
	};
}