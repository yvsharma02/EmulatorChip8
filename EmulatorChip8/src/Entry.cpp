#include "pch.h"
#include "stdio.h"
#include "Window/C8Window.h"
#include <math.h>

#include <iostream>

#if PLATFORM_WINDOWS

Chip8::C8Window* main_window;

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
    /*
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
    */
}

void onWindowEvent(Chip8::C8EventType type)
{
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    int unscaled_size_x = 128;
    int unscaled_size_y = 128;
    main_window = new Chip8::C8Window(L"Chip8 Emulator", unscaled_size_x, unscaled_size_y, hInstance, nCmdShow);
    
    main_window->add_window_event_listener(onWindowEvent);

//    c8_event_listener listener = &onWindowEvent;
//    window.add_window_event_listener(listener);

    set_colors(unscaled_size_x, unscaled_size_y);

    main_window->run_for_windows();

    delete main_window;

    return 0;
}

#endif