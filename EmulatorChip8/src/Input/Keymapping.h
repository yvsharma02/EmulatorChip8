#pragma once

#include "pch.h"
#include "Input/Keycode.h"
#include "Input/Keystate.h"

namespace Chip8
{
	class C8Keymapping
	{

	public:
		C8Keymapping(int actual, Chip8::C8Keycode::C8Keycode mapped) : actual_key(actual), mapped_key(mapped), state(C8Keystate::NONE) {}

		inline int get_actual_key() const { return actual_key; }
		inline Chip8::C8Keycode::C8Keycode get_mapped_key() const { return mapped_key; }

		inline C8Keystate::C8Keystate get_state() { return state; }
		inline void set_state(C8Keystate::C8Keystate new_state) { state = new_state; }
		 
	private:
		int actual_key;
		C8Keystate::C8Keystate state;
		Chip8::C8Keycode::C8Keycode mapped_key;

	};
}