#pragma once

#include "pch.h"
#include "Memory/C8Memory.h"

namespace Chip8
{
	bool read_rom(std::wstring file_name, C8Memory& mem, c8short write_start_loc);
}