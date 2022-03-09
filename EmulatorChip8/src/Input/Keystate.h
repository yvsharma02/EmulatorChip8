#pragma once
#include "pch.h"

namespace Chip8::C8Keystate
{
	enum C8Keystate : c8byte
	{
		PRESSED = 0,
		CONTINUED_PRESS,
		RELEASED,
		NONE
	};
}