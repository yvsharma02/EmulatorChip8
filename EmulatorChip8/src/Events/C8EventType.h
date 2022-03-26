#pragma once

namespace Chip8
{
	enum class C8EventType
	{
		WINDOW_RESCALE,
		KEYBOARD_INPUT,
		UPDATE,
		CLOCK_TICK,
		LOAD_ROM
	};
}