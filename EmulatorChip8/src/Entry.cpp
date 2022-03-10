#include "pch.h"
#include "stdio.h"
#include "Window/C8Window.h"
#include "IO/IO.h"
#include "Memory/C8Memory.h"
#include <math.h>

#include <iostream>

#if PLATFORM_WINDOWS

Chip8::C8Window* main_window;

void set_colors_2(int unscaled_size_x, int unscaled_size_y)
{
    double maxDist = sqrt(unscaled_size_x * unscaled_size_x + unscaled_size_y * unscaled_size_y) / 2.0;

    Chip8::c8byte* colors = new Chip8::c8byte[unscaled_size_x * unscaled_size_y];
    int c = 0;

    for (int i = 0; i < unscaled_size_y; i++)
    {
        for (int j = 0; j < unscaled_size_x; j++)
        {
            double distRatio = 1 - sqrt((unscaled_size_x / 2 - i) * (unscaled_size_x / 2 - i) + (unscaled_size_y / 2 - j) * (unscaled_size_y / 2 - j)) / maxDist;
            colors[c++] = (Chip8::c8byte)(distRatio * 255);
        }
    }

    main_window->set_colors(colors);

    delete[] colors;
}

void set_colors(int unscaled_size_x, int unscaled_size_y)
{
    Chip8::c8byte* clrs = new Chip8::c8byte[unscaled_size_x * unscaled_size_y];
    int c = 0;

    for (int i = 0; i < unscaled_size_y; i++)
    {
        for (int j = 0; j < unscaled_size_x; j++)
        {
            clrs[c++] = (float)(i) / unscaled_size_y * 255;
        }
    }
    
    main_window->set_colors(clrs);

    delete[] clrs;
}

void on_keyboard_event(Chip8::C8EventType type, void* data)
{
    int unscaled_size_x = 64;
    int unscaled_size_y = 32;

    Chip8::c8byte* info = (Chip8::c8byte*)data;

    if (info[1] == Chip8::C8Keycode::A && info[0] == Chip8::C8Keystate::RELEASED)
        set_colors(unscaled_size_x, unscaled_size_y);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{

    int unscaled_size_x = 64;
    int unscaled_size_y = 32;
    main_window = new Chip8::C8Window(L"Chip8 Emulator", unscaled_size_x, unscaled_size_y, hInstance, nCmdShow);
    
    main_window->add_keyboard_event_listener(on_keyboard_event);

    Chip8::C8Keymapping key_mapping[16] =
    {
        Chip8::C8Keymapping::C8Keymapping(0x34, Chip8::C8Keycode::ONE),     // 4
        Chip8::C8Keymapping::C8Keymapping(0x35, Chip8::C8Keycode::TWO),     // 5
        Chip8::C8Keymapping::C8Keymapping(0x36, Chip8::C8Keycode::THREE),   // 6
        Chip8::C8Keymapping::C8Keymapping(0x37, Chip8::C8Keycode::C),       // 7

        Chip8::C8Keymapping::C8Keymapping(0x52, Chip8::C8Keycode::FOUR),    // R
        Chip8::C8Keymapping::C8Keymapping(0x54, Chip8::C8Keycode::FIVE),    // T
        Chip8::C8Keymapping::C8Keymapping(0x59, Chip8::C8Keycode::SIX),     // Y
        Chip8::C8Keymapping::C8Keymapping(0x55, Chip8::C8Keycode::D),       // U

        Chip8::C8Keymapping::C8Keymapping(0x46, Chip8::C8Keycode::SEVEN),   // F
        Chip8::C8Keymapping::C8Keymapping(0x47, Chip8::C8Keycode::EIGHT),   // G
        Chip8::C8Keymapping::C8Keymapping(0x48, Chip8::C8Keycode::NINE),    // H
        Chip8::C8Keymapping::C8Keymapping(0x4A, Chip8::C8Keycode::E),       // J

        Chip8::C8Keymapping::C8Keymapping(0x56, Chip8::C8Keycode::A),       // V
        Chip8::C8Keymapping::C8Keymapping(0x42, Chip8::C8Keycode::ZERO),    // B
        Chip8::C8Keymapping::C8Keymapping(0x4E, Chip8::C8Keycode::B),       // N
        Chip8::C8Keymapping::C8Keymapping(0x4D, Chip8::C8Keycode::F),       // N
    };

    main_window->set_key_map(key_mapping, 16);

    set_colors_2(unscaled_size_x, unscaled_size_y);

    Chip8::C8Memory mem = Chip8::C8Memory();
    bool file_read_success = Chip8::read_rom(L"../TestROMs/chip8-test-rom.ch8", mem, 0x200);

    main_window->run_for_windows();

    delete main_window;

    return 0;
}

#endif