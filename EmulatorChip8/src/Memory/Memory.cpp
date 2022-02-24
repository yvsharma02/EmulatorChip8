#include "pch.h"
#include "Memory.h"

namespace Chip8
{
	c8byte& Memory::operator[] (int address)
	{
		if (address < 0 || address >= dataSize)
			throw new std::out_of_range("address is out of range");

		return data[address];
	}

	Memory::Memory()
	{
		dataSize = C8_MEMORY_SIZE;
		data = new c8byte[C8_MEMORY_SIZE];
	}

	Memory::~Memory()
	{
		delete data;
	}

	int Memory::size() const
	{
		return dataSize;
	}
};