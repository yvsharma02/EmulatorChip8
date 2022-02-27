#include "pch.h"
#include "stdio.h"
#include "Window/C8Window.h"

#include <iostream>

#if PLATFORM_WINDOWS

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    auto window = Chip8::C8Window(L"Chip8 Emulator", 512, 512, hInstance, nCmdShow);
    
    Chip8::c8byte* colors = new Chip8::c8byte[512 * 512];
    int c = 0;

    for (int i = 0; i < 512; i++)
        for (int j = 0; j < 512; j++)
            colors[c++] = c % 255;

    window.set_colors(colors);
    delete colors;

    window.run();

    return 0;
}

#endif