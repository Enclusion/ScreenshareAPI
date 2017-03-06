//Copyright 2017, Ghose, All rights reserved.
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

#include "dllmain.h"
#include "memory.h"

//===============================================================================================//
BOOL CALLBACK GetWindowsOfProcessProc(HWND hwnd, LPARAM lParam)
{
	PGWOP_CTX ctx;
	DWORD processId;

	ctx = (PGWOP_CTX)lParam;
	GetWindowThreadProcessId(hwnd, &processId);

	if (processId == ctx->TargetProcess)
	{
		HWND* newMemory = NULL;

		newMemory = (HWND*)MemReAlloc(ctx->Windows, (ctx->WindowCount + 1) * sizeof(HWND));
		if (NULL == newMemory)
			return FALSE;

		ctx->Windows = newMemory;
		ctx->Windows[ctx->WindowCount++] = hwnd;
	}

	return TRUE;
}
//===============================================================================================//

//===============================================================================================//
PGWOP_CTX GetWindowsOfProcess(DWORD pid)
{
	PGWOP_CTX ctx;

	if ((ctx = (PGWOP_CTX)MemCalloc(sizeof(GWOP_CTX))))
	{
		ctx->TargetProcess = pid;
		EnumWindows(GetWindowsOfProcessProc, (LPARAM)ctx);
		return ctx;
	}

	return NULL;
}
//===============================================================================================//

//===============================================================================================//
PFPBNAWT_CTX FindProcessByNameAndWindowTitle(const wchar_t* name, const wchar_t* title, const BOOL matchTitleExact)
{
	HANDLE snapshot = INVALID_HANDLE_VALUE;
	PFPBNAWT_CTX fpbnawtCtx = NULL;

	if (INVALID_HANDLE_VALUE != (snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)))
	{
		PROCESSENTRY32 entry = { 0 };
		PDWORD pids = NULL;
		SIZE_T pidCount = 0;

		entry.dwSize = sizeof(PROCESSENTRY32);

		if (TRUE == Process32First(snapshot, &entry))
		{
			while (TRUE == Process32Next(snapshot, &entry))
			{
				if (0 == wcscmp(entry.szExeFile, name))
				{
					PDWORD newMemory = NULL;

					newMemory = (PDWORD)MemReAlloc(pids, (pidCount + 1) * sizeof(DWORD));
					if (NULL == newMemory)
						break;

					pids = newMemory;
					pids[pidCount++] = entry.th32ProcessID;
				}
			}
		}

		if (NULL != pids)
		{
			fpbnawtCtx = (PFPBNAWT_CTX)MemCalloc(sizeof(FPBNAWT_CTX));

			if (NULL != fpbnawtCtx)
			{
				PGWOP_CTX gwopCtx;
				wchar_t textBuf[256];
				BOOL outOfMemory = FALSE;

				for (SIZE_T iii = 0; iii < pidCount; iii++)
				{
					gwopCtx = GetWindowsOfProcess(pids[iii]);

					if (gwopCtx == NULL)
						// Failed to allocate memory
						break;

					for (SIZE_T jjj = 0; jjj < gwopCtx->WindowCount; jjj++)
					{
						ZeroMemory(textBuf, sizeof(textBuf));

						if (GetWindowText(gwopCtx->Windows[jjj], textBuf, sizeof(textBuf)) > 0)
						{
							BOOL match = FALSE;

							if (matchTitleExact)
							{
								if (0 == wcscmp(textBuf, title))
								{
									match = TRUE;
								}
							}
							else
							{
								if (NULL != wcsstr(textBuf, title))
								{
									match = TRUE;
								}
							}

							if (TRUE == match)
							{
								PDWORD newMemory = NULL;

								newMemory = (PDWORD)MemReAlloc(fpbnawtCtx->Pids, (fpbnawtCtx->PidCount + 1) * sizeof(DWORD));
								if (NULL == newMemory) {
									outOfMemory = TRUE;
									break;
								}

								fpbnawtCtx->Pids = newMemory;
								fpbnawtCtx->Pids[fpbnawtCtx->PidCount++] = pids[iii];
								break;
							}
						}
					}

					GWOP_CTX_FREE(gwopCtx);

					if (outOfMemory)
						break;
				}
			}

			MemFree(pids);
			pids = NULL;
		}
	}

	return fpbnawtCtx;
}
//===============================================================================================//

//===============================================================================================//
PVOID GetLibraryProcAddress(PSTR LibraryName, PSTR ProcName)
{
	return GetProcAddress(GetModuleHandleA(LibraryName), ProcName);
}
//===============================================================================================//

//===============================================================================================//
VOID StringSearch(DWORD pId, STRING_VEC strings)
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

		BYTE *buffer = (BYTE *)MemAlloc(memoryInfo.RegionSize);

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

		MemFree(buffer);

		address += memoryInfo.RegionSize;
	}

	std::cout << "Credits: Ghose(buzznacker) & Fyu" << std::endl;

	CloseHandle(hProcess);
}
//===============================================================================================//

//===============================================================================================//
DWORD FindProcessId(std::wstring processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processSnapshot);
	return 0;
}
//===============================================================================================//

//===============================================================================================//
//    https://github.com/giampaolo/psutil/blob/master/psutil/arch/windows/process_handles.c      //
//===============================================================================================//
STRING_VEC GetProcessHandles(DWORD pId)
{
	HANDLE hProcess;
	NTSTATUS status;
	PSYSTEM_HANDLE_INFORMATION handleInfo;
	ULONG handleInfoSize = 0x10000;
	STRING_VEC vec;

	hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pId);

	if (INVALID_HANDLE_VALUE == hProcess)
	{
		std::cout << "Cannot open target process, make sure it is still active." << std::endl;
		return vec;
	}

	_NtQuerySystemInformation NtQuerySystemInformation = (_NtQuerySystemInformation)GetLibraryProcAddress("ntdll.dll", "NtQuerySystemInformation");
	_NtDuplicateObject NtDuplicateObject = (_NtDuplicateObject)GetLibraryProcAddress("ntdll.dll", "NtDuplicateObject");
	_NtQueryObject NtQueryObject = (_NtQueryObject)GetLibraryProcAddress("ntdll.dll", "NtQueryObject");

	handleInfo = (PSYSTEM_HANDLE_INFORMATION)MemAlloc(handleInfoSize);

	while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, NULL)) == STATUS_INFO_LENGTH_MISMATCH)
		handleInfo = (PSYSTEM_HANDLE_INFORMATION)MemReAlloc(handleInfo, handleInfoSize *= 2);

	/* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
	if (!NT_SUCCESS(status))
	{
		std::cout << "An error has occured while attempting to query system information." << std::endl;
		return vec;
	}

	for (int i = 0; i < handleInfo->HandleCount; i++)
	{
		SYSTEM_HANDLE handle = handleInfo->Handles[i];
		HANDLE dupHandle = NULL;
		POBJECT_TYPE_INFORMATION objectTypeInfo;
		PVOID objectNameInfo;
		UNICODE_STRING objectName;
		ULONG returnLength;

		if (handle.ProcessId != pId)
			continue;

		// We must duplicate the handle to query it.
		if (!NT_SUCCESS(DuplicateHandle(hProcess, (HANDLE)handle.Handle, GetCurrentProcess(), &dupHandle, 0, 0, 0)))
			continue;

		objectTypeInfo = (POBJECT_TYPE_INFORMATION)MemAlloc(0x1000);
		if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, NULL)))
		{
			CloseHandle(dupHandle);
			continue;
		}

		if (handle.GrantedAccess == 0x0012019f)
		{
			MemFree(objectTypeInfo);
			CloseHandle(dupHandle);
			continue;
		}

		objectNameInfo = MemAlloc(0x1000);
		if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, 0x1000, &returnLength)))
		{
			//If it failed we simply try again by reallocating the buffer.
			objectNameInfo = MemReAlloc(objectNameInfo, returnLength);
			if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, returnLength, NULL)))
			{
				MemFree(objectTypeInfo);
				MemFree(objectNameInfo);
				CloseHandle(dupHandle);
				continue;
			}
		}

		objectName = *(PUNICODE_STRING)objectNameInfo;
		if (objectName.Length)
		{
			char str[255];
			WideCharToMultiByte(CP_ACP, 0, objectName.Buffer, -1, str, sizeof(str), NULL, NULL);
			vec.push_back(str);
		}

		MemFree(objectTypeInfo);
		MemFree(objectNameInfo);
		CloseHandle(dupHandle);
	}

	MemFree(handleInfo);
	CloseHandle(hProcess);
	return vec;
}
//===============================================================================================//

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


