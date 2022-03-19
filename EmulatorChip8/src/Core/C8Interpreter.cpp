#include "C8Interpreter.h"

namespace Chip8
{
	C8Interpreter::C8Interpreter(C8Window& output_window) : output_window(output_window), memory(C8Memory())
	{
		stack_pointer = STACK_START;
		program_counter = PROGRAM_MEM_START;

		display_buffer = new bool[UNSCALED_HEIGHT * UNSCALED_WIDTH];

		register_16 = 0;
		keys_pressed = 0;

		for (int i = 0; i < 16; i++)
			V[i] = 0;

		paused = true;

		load_text_sprites();
		clear_screen();
	}

	c8byte C8Interpreter::get_instruction(c8byte high, c8byte low)
	{
		return high >> 4;
	}

	int C8Interpreter::get_addr(c8byte high, c8byte low)
	{
		int x = ((((int)high) << 8) & 0x0F00);
		return x | low;
	}

	c8byte C8Interpreter::get_nible(c8byte high, c8byte low)
	{
		return low & 0b00001111;
	}

	c8byte C8Interpreter::get_x(c8byte high, c8byte low)
	{
		return high & 0b00001111;
	}

	c8byte C8Interpreter::get_y(c8byte high, c8byte low)
	{
		return (low & 0b11110000) >> 4;
	}

	c8byte C8Interpreter::get_kk(c8byte high, c8byte low)
	{
		return low;
	}

	void C8Interpreter::skip_next_instruction()
	{
		set_pc(get_pc() + 4);
	}

	void C8Interpreter::increment_pc()
	{
		set_pc(get_pc() + 2);
	}

	int C8Interpreter::get_pc()
	{
		return program_counter;
	}

	void C8Interpreter::set_pc(int addr)
	{
		program_counter = addr;
	}

	c8byte C8Interpreter::generate_random_byte()
	{
		return rand();
	}

	bool C8Interpreter::is_key_pressed(C8Keycode::C8Keycode keycode)
	{
		return keys_pressed & (1 << keycode);
	}

	void C8Interpreter::trigger_update()
	{
		if (paused)
			return;

		

//		Sleep(1000);
		run();
	}

	void C8Interpreter::trigger_tick()
	{
//		if (delay_register > 0)
//			delay_register -= 1;
	}

	bool C8Interpreter::load_rom(const std::wstring& loc)
	{
		return Chip8::read_rom(loc, memory, PROGRAM_MEM_START);
	}

	void C8Interpreter::run()
	{
		if (delay_register > 0)
			delay_register -= 1;

		c8byte high_byte = memory[get_pc()];
		c8byte low_byte = memory[get_pc() + 1];

		c8byte instruction = get_instruction(high_byte, low_byte);
		c8short addr = get_addr(high_byte, low_byte);
		c8byte nibble = get_nible(high_byte, low_byte);
		c8byte x = get_x(high_byte, low_byte);
		c8byte y = get_y(high_byte, low_byte);
		c8byte kk = get_kk(high_byte, low_byte);

		switch (instruction)
		{
		case 0:
			if (y == 0xE && nibble == 0)
				clear_screen();
			else if (kk == 0xEE)
			{
				pop_stack();

				int high = memory[stack_pointer];
				int low = memory[stack_pointer + 1];

				int addr_stack_top = (high << 8) | low;
				set_pc(addr_stack_top);
				break;
			}
			break;

		case 0x1:
			set_pc(addr);
			return;

		case 0x2:
			memory[stack_pointer] = (c8byte) (program_counter >> 8);
			memory[stack_pointer + 1] = (c8byte) program_counter;
			push_stack();

			set_pc(addr);
			return;

		case 0x3:
			if (V[x] == kk)
			{
				skip_next_instruction();
				return;
			}
			break;

		case 0x4:
			if (V[x] != kk)
			{
				skip_next_instruction();
				return;
			}
			break;

		case 0x5:
			if (V[x] == V[y])
			{
				skip_next_instruction();
				return;
			}
			break;

		case 0x6:
			V[x] = kk;
			break;

		case 0x7:
			V[x] += kk;
			break;

		case 0x8:
			switch (nibble)
			{
			case 0:
				V[x] = V[y];
				break;
			case 1:
				V[x] |= V[y];
				break;
			case 2:
				V[x] &= V[y];
				break;
			case 3:
				V[x] ^= V[y];
				break;
			case 4:
			{
				int sum = V[x] + V[y];
				V[x] = sum;
				V[0xF] = sum >> 8;

				break;
			}
			case 5:
				V[0xF] = V[x] > V[y];
				V[x] -= V[y];
				break;

			case 6:
				V[0xF] = V[x] & 0x01;
				V[x] = V[x] >> 1;
				break;

			case 7:
				V[0xF] = V[y] > V[x];
				V[x] = V[y] - V[x];
				break;

			case 0xE:
				V[0xF] = ((V[x] & 0b10000000) != 0) ? 1 : 0;
				V[x] = V[x] << 1;
				break;

			default:
				return;
			}
			break;

		case 9:
			if (V[x] != V[y])
			{
				skip_next_instruction();
				return;
			}
			break;

		case 0xA:
			register_16 = addr;
			break;

		case 0xB:
			set_pc(addr + V[0]);
			return;

		case 0xC:
			V[x] = kk & generate_random_byte();
			break;

		case 0xD:
		{
			V[0xF] = 0;
			int x_coord = V[x] % UNSCALED_WIDTH;
			int y_coord = V[y] % UNSCALED_HEIGHT;

			for (int i = 0; i < nibble; i++)
			{
				if (y_coord + i >= UNSCALED_HEIGHT)
					break;

				c8byte row = memory[register_16 + i];

				for (int j = 0; j < DEFAULT_SPRITE_WIDTH; j++)
				{
					if (x_coord + j >= UNSCALED_WIDTH)
						break;

					int index = (y_coord + i) * UNSCALED_WIDTH + (x_coord + j);

					bool open_now = (row & (0b10000000 >> j)) != 0;
					bool was_open = display_buffer[index];

					display_buffer[index] = open_now ^ was_open;

					if (!display_buffer[index] && was_open)
						V[0xF] = 1;
				}

				update_display();
			}
		}
			break;

		case 0xE:
		{
			switch (kk)
			{
			case 0x9E:
				if (is_key_pressed((C8Keycode::C8Keycode)V[x]))
				{
					skip_next_instruction();
					return;
				}
				break;

			case 0xA1:
				if (!is_key_pressed((C8Keycode::C8Keycode)V[x]))
				{
					skip_next_instruction();
					return;
				}
				break;

			default:
				return;
			}
		}
		break;

		case 0xF:

			switch (kk)
			{
			case 0x07:
				V[x] = delay_register;
				break;

			case 0x0A:
//				while (!keys_pressed); Cant use a while loop since it will block windows messages. Hence the key will never be detected anyway.

				if (!keys_pressed)
					return;

				for (int i = 0; i < 16; i++)
				{
					if (is_key_pressed((C8Keycode::C8Keycode)i))
					{
						V[x] = i;
						break;
					}
				}
				break;

			case 0x15:
				delay_register = V[x];
				break;

			case 0x18:
				sound_timer_register = V[x];
				break;

			case 0x1E:
				register_16 += V[x];
				break;

			case 0x29:
				register_16 = TEXT_SPRITE_MEM_STRART + V[x] * TEXT_SPRITE_SIZE_BYTES;
				break;

			case 0x33:
			{
				c8byte hundereds = V[x] / 100;
				c8byte tens = (V[x] / 10) % 10;
				c8byte ones = V[x] % 10;

				memory[register_16] = hundereds;
				memory[register_16 + 1] = tens;
				memory[register_16 + 2] = ones;
				break;
			}
			case 0x55:
			{
				for (int i = 0; i <= x; i++)
					memory[register_16 + i] = V[i];
				break;
			}
			case 0x65:
			{
				for (int i = 0; i <= x; i++)
					V[i] = memory[register_16 + i];
				break;
			}

			default:
				return;
			}
			break;

		default:
			return;
		}
		increment_pc();
	}

	c8byte* C8Interpreter::get_memory_dump()
	{
		c8byte* dump = new c8byte[C8_MEMORY_SIZE];

		for (int i = 0; i < C8_MEMORY_SIZE; i++)
			dump[i] = memory[i];

		return dump;
	}

	void C8Interpreter::trigger_key(C8Keystate::C8Keystate keystate, C8Keycode::C8Keycode keycode)
	{
		if (keystate == Chip8::C8Keystate::PRESSED)
			keys_pressed |= (1 << keycode);

		if (keystate == Chip8::C8Keystate::RELEASED)
			keys_pressed &= ~(1 << keycode);

		play();
	}

	void C8Interpreter::pause()
	{
		paused = true;
	}

	void C8Interpreter::play()
	{
		paused = false;
	}

	void C8Interpreter::update_display()
	{
		output_window.set_colors(display_buffer);
	}

	void C8Interpreter::clear_screen()
	{
		int c = 0;
		for (int i = 0; i < UNSCALED_HEIGHT; i++)
			for (int j = 0; j < UNSCALED_WIDTH; j++)
				display_buffer[c++] = true;

		update_display();
	}

	void C8Interpreter::pop_stack()
	{
		stack_pointer -= 2;
	}

	void C8Interpreter::push_stack()
	{
		stack_pointer += 2;
	}

	void C8Interpreter::load_text_sprites()
	{
		c8byte sprites[TEXT_SPRITE_SIZE_BYTES * TEXT_SPRITE_COUNT]
		{
			0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
			0x20, 0x60, 0x20, 0x20, 0x70,	// 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
			0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
			0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
			0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
			0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
			0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
			0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
			0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
			0xF0, 0x80, 0xF0, 0x80, 0x80	// F
		};

		for (int i = 0; i < TEXT_SPRITE_SIZE_BYTES * TEXT_SPRITE_COUNT; i++)
			memory[TEXT_SPRITE_MEM_STRART + i] = sprites[i];
	}

	C8Interpreter::~C8Interpreter()
	{
		delete[] display_buffer;
	}
}