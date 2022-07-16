#include "pch.h"
#include "stdio.h"
#include "Window/C8Window.h"
#include "IO/IO.h"
#include "Core/C8Interpreter.h"

#include <math.h>
#include <iostream>
#include <fstream>

#if PLATFORM_WINDOWS

Chip8::C8Window* main_window;
Chip8::C8Interpreter* interpreter;

void on_keyboard_event(Chip8::C8EventType type, void* data)
{
    Chip8::c8byte* keystate_code = (Chip8::c8byte*) data;
    interpreter->trigger_key((Chip8::C8Keystate::C8Keystate) keystate_code[0], (Chip8::C8Keycode::C8Keycode)keystate_code[1]);
}

void on_update(Chip8::C8EventType type, void* data)
{
    interpreter->trigger_update();
}

void on_load_rom(Chip8::C8EventType type, void* data)
{
    WCHAR* file_name = (WCHAR*)data;
    interpreter->load_rom(std::wstring(file_name));
    interpreter->play();
}


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    main_window = new Chip8::C8Window(L"Chip8 Emulator", Chip8::DEFAULT_WINDOW_WIDTH, Chip8::DEFAULT_WINDOW_HEIGHT, hInstance, nCmdShow);

    main_window->add_keyboard_event_listener(on_keyboard_event);

    Chip8::C8Keymapping key_mapping[16] =
    {
        Chip8::C8Keymapping::C8Keymapping(0x31, Chip8::C8Keycode::ONE),     // 1
        Chip8::C8Keymapping::C8Keymapping(0x32, Chip8::C8Keycode::TWO),     // 2
        Chip8::C8Keymapping::C8Keymapping(0x33, Chip8::C8Keycode::THREE),   // 3
        Chip8::C8Keymapping::C8Keymapping(0x34, Chip8::C8Keycode::C),       // 4

        Chip8::C8Keymapping::C8Keymapping(0x51, Chip8::C8Keycode::FOUR),    // Q
        Chip8::C8Keymapping::C8Keymapping(0x57, Chip8::C8Keycode::FIVE),    // W
        Chip8::C8Keymapping::C8Keymapping(0x45, Chip8::C8Keycode::SIX),     // E
        Chip8::C8Keymapping::C8Keymapping(0x52, Chip8::C8Keycode::D),       // R

        Chip8::C8Keymapping::C8Keymapping(0x41, Chip8::C8Keycode::SEVEN),   // A
        Chip8::C8Keymapping::C8Keymapping(0x53, Chip8::C8Keycode::EIGHT),   // S
        Chip8::C8Keymapping::C8Keymapping(0x44, Chip8::C8Keycode::NINE),    // D
        Chip8::C8Keymapping::C8Keymapping(0x46, Chip8::C8Keycode::E),       // F

        Chip8::C8Keymapping::C8Keymapping(0x5A, Chip8::C8Keycode::A),       // Z
        Chip8::C8Keymapping::C8Keymapping(0x58, Chip8::C8Keycode::ZERO),    // X
        Chip8::C8Keymapping::C8Keymapping(0x43, Chip8::C8Keycode::B),       // C
        Chip8::C8Keymapping::C8Keymapping(0x56, Chip8::C8Keycode::F),       // V
    };

    main_window->set_key_map(key_mapping, 16);

    interpreter = new Chip8::C8Interpreter(*main_window);
//    bool load_successful = interpreter->load_rom(L"../TestROMs/pong.ch8");

    main_window->add_update_event_listener(on_update);
    main_window->add_load_rom_event_listener(on_load_rom);

    main_window->run();

    delete main_window;
    delete interpreter;

    return 0;
}

Chip8::C8Window* get_window()
{
    return main_window;
}

#endif