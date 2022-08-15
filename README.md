# EmulatorChip8
A simple Chip-8 Emulator written for Windows in C++, using Win32 API.
<br/>

My main goal for this project was to learn C++ (especially manual memory management), which is why I've used raw pointers instead of smart pointers, and avoided using built-in std library (<i> again, just as a learning experience </i>).

 Coming from Object-Oriented Languages (especially C#), I tried to follow the similar (especially events) which was not ideal. Because  This lead to some messy code. (For example: needing public methods in a class to trigger events, instead of them being triggered privately. This was mainly caused because a method of a class has a different type than a global method).


<img src="https://i.imgur.com/fFN2IEV.mp4" width="512" height="512" />

Games used for testing: https://github.com/kripod/chip8-roms/tree/master/games <br/>

Other ROMS used specifically for testing: <br/>
https://github.com/metteo/chip8-test-rom <br>
https://github.com/corax89/chip8-test-rom