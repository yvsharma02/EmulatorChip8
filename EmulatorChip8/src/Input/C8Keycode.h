#pragma once

#include "pch.h"

#define C8_KEYCODE_COUNT 16

namespace Chip8::C8Keycode
{
	enum C8Keycode : c8byte
	{
		ZERO = 0,
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		A,
		B,
		C,
		D,
		E,
		F,
	};
};