#pragma once

#include <string>
#include <stdexcept>

#if PLATFORM_WINDOWS

#include <Windows.h>

#endif

namespace Chip8
{
	using c8byte = uint8_t;
}