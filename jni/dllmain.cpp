//Copyright 2017, Ghose, All rights reserved.
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

#include "dllmain.h"

VOID StringSearch(DWORD pId, std::vector<std::string> strings)
{
	HANDLE hProcess;
	BYTE *address;
	MEMORY_BASIC_INFORMATION memoryInfo;

	hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pId);
	address = 0;

	if (INVALID_HANDLE_VALUE == hProcess)
	{
		std::cout << "Cannot access the target process." << std::endl;
		return;
	}

	while (VirtualQueryEx(hProcess, address, &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo))
	{
		address = (BYTE *)memoryInfo.BaseAddress;

		BYTE *buffer = (BYTE *)malloc(memoryInfo.RegionSize);

		if (memoryInfo.Protect != PAGE_NOACCESS && memoryInfo.State == MEM_COMMIT && memoryInfo.Type == MEM_PRIVATE)
		{
			if (ReadProcessMemory(hProcess, address, buffer, memoryInfo.RegionSize, 0))
			{
				for (int i = 0; i < memoryInfo.RegionSize - 4; i++)
				{
					for (int j = 0; j < strings.size(); j++)
					{
						if (memcmp(&buffer[i], &strings[j], strings[j].length()) == 0)
						{
							std::cout << "found string #" << j << " in the memory" << std::endl;
						}
					}
				}
			}
		}

		free(buffer);

		address += memoryInfo.RegionSize;
	}

	std::cout << "Credits: Ghose(buzznacker) & Fyu" << std::endl;

	CloseHandle(hProcess);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		std::cout << "Attached to process." << std::endl;
	}
	return TRUE;
}
//Copyright 2017, Ghose, All rights reserved.


