# EmulatorChip8
A simple Chip-8 Emulator written for Windows in C++, using Win32 API.
<br/>

My main goal for this project was to learn C++ (especially manual memory management), which is why I've used raw pointers instead of smart pointers, and avoided using built-in std library (<i> again, just as a learning experience </i>).

 Coming from Object-Oriented Languages (especially C#), I tried to follow the similar (especially events) which was not ideal. Because  This lead to some messy code. (For example: needing public methods in a class to trigger events, instead of them being triggered privately. This was mainly caused because a method of a class has a different type than a global method).

Preview(s):


https://user-images.githubusercontent.com/95173175/184576385-3fdecfa0-4d86-44f0-8b18-8cd3f6e9d601.mp4


https://user-images.githubusercontent.com/95173175/184576379-ff45c7ab-7b11-410b-96aa-f3ec21ce10b4.mp4


https://user-images.githubusercontent.com/95173175/184576384-79e66c0d-50c9-4ccb-8a4d-f1921624696b.mp4


https://user-images.githubusercontent.com/95173175/184576386-dd6b5f48-a32e-4fea-b898-c66dfce18b90.mp4

(Please note: Each game is running on a different clock speed.)

Games used for testing: https://github.com/kripod/chip8-roms/tree/master/



Other ROMS used specifically for testing: <br/>
https://github.com/metteo/chip8-test-rom <br>
https://github.com/corax89/chip8-test-rom