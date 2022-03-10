#include "pch.h"
#include "C8Memory.h"

namespace Chip8
{
	c8byte& C8Memory::operator[] (int address)
	{
		if (address < 0 || address >= dataSize)
			throw new std::out_of_range("address is out of range");

		return data[address];
	}

	C8Memory::C8Memory()
	{
		dataSize = C8_MEMORY_SIZE;
		data = new c8byte[C8_MEMORY_SIZE];
	}

	C8Memory::~C8Memory()
	{
		delete[] data;
	}

	int C8Memory::size() const
	{
		return dataSize;
	}
};