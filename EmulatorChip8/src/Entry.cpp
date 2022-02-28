#include "pch.h"
#include "stdio.h"
#include "Window/C8Window.h"
#include <math.h>

#include <iostream>

#if PLATFORM_WINDOWS

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    int unscaled_size_x = 512;
    int unscaled_size_y = 512;
    auto window = Chip8::C8Window(L"Chip8 Emulator", unscaled_size_x, unscaled_size_y, hInstance, nCmdShow);
    
    double maxDist = sqrt(unscaled_size_x * unscaled_size_x + unscaled_size_y * unscaled_size_y) / 2.0;

    Chip8::c8byte* colors = new Chip8::c8byte[unscaled_size_x * unscaled_size_y];
    int c = 0;

    for (int i = 0; i < unscaled_size_x; i++)
    {
        for (int j = 0; j < unscaled_size_y; j++)
        {
            double distRatio = 1 - sqrt((unscaled_size_x / 2 - i) * (unscaled_size_x / 2 - i) + (unscaled_size_y / 2 - j) * (unscaled_size_y / 2 - j)) / maxDist;
            colors[c++] = (Chip8::c8byte)(distRatio * 255);
        }
    }
    window.set_colors(colors);
    delete colors;

    window.run_for_windows();

    return 0;
}

#endif