#include "pch.h"
#include "stdio.h"
#include "Window/C8Window.h"

#include <iostream>

#if PLATFORM_WINDOWS

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    auto window = Chip8::C8Window(L"Chip8 Emulator", 512, 512, hInstance, nCmdShow);
    window.run();
    return 0;
}

#endif