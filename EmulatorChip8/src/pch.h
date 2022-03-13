#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#if PLATFORM_WINDOWS

#include <Windows.h>

#endif


namespace Chip8
{
	constexpr int UNSCALED_HEIGHT = 64;
	constexpr int UNSCALED_WDITH = 32;

	constexpr int DEFAULT_WINDOW_HEIGHT = 512;
	constexpr int DEFAULT_WINDOW_WIDTH = 512;

	using c8byte = uint8_t;
	using c8short = uint16_t;

#if PLATFORM_WINDOWS
	using c8Color = COLORREF;
#else
	using c8Color = c8byte;
#endif
}