#include "IO.h"

namespace Chip8
{
#if PLATFORM_WINDOWS

	bool Chip8::read_rom(std::wstring file_name, C8Memory& mem, c8short write_start_loc)
	{
		HANDLE hFile = CreateFile(file_name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		const short read_buffer_size = C8_MEMORY_SIZE;
		
		int total_bytes_read = 0;

		DWORD bytes_read = 1;
		c8byte read_buffer[read_buffer_size];

		bool read_success = true;

		while (bytes_read > 0)
		{
			read_success = ReadFile(hFile, read_buffer, read_buffer_size, &bytes_read, NULL);
			
			if (!read_success)
			{
				CloseHandle(hFile);
				return false;
			}
			total_bytes_read += bytes_read;
		}

		CloseHandle(hFile);
		
		if (!read_success)
			return false;
		
		for (int i = 0; i < total_bytes_read; i++)
			mem[write_start_loc + i] = read_buffer[i];

		return true;
	}

#endif
}