#include "stdafx.h"
#include "externaltools.h"
using namespace std; 

DWORD GetProcID(const wchar_t * name) //gets process ID based on process name
{
	DWORD ID = 0;
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (h != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);
		if (Process32First(h, &entry))
		{
			do
			{
				if (!wcsicmp(entry.szExeFile, name))
				{
					ID = entry.th32ProcessID;
					break;
				}
			} while (Process32Next(h, &entry));
		}

	}
	CloseHandle(h);
	return ID;
}


uintptr_t  GetModuleBaseAddress(DWORD procID, const wchar_t * ModuleName) //gets base address of processes or modules. 
{
	uintptr_t ModuleBaseAddress = 0;
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (h != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry;
		ModuleEntry.dwSize = sizeof(ModuleEntry);
		if (Module32First(h, &ModuleEntry))
		{
			do
			{
				if (!wcsicmp(ModuleEntry.szModule, ModuleName))
				{
					ModuleBaseAddress = (uintptr_t)ModuleEntry.modBaseAddr;
				}
			} while (Module32Next(h, &ModuleEntry));
		}
	}
	CloseHandle(h);
	return ModuleBaseAddress;
}

int PrintModules(DWORD processID)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	// Print the process identifier.

	printf("\nProcess ID: %u\n", processID);

	// Get a handle to the process.

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return 1;

	// Get a list of all the modules in this process.

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.

				_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
			}
		}
	}

	// Release the handle to the process.

	CloseHandle(hProcess);

	return 0;
}



//gets the addresses of interesting values. 
uintptr_t GetWriteAddress(HANDLE h, uintptr_t moduleBase, vector<UINT> offsets) //first offset must be base + offsets[i] 
{
	uintptr_t addr = moduleBase + offsets[0];
	for (int i = 1; i < offsets.size(); ++i)
	{
		ReadProcessMemory(h, (BYTE*)addr, &addr, sizeof(addr), NULL);
		addr = addr + offsets[i];
	}
	return addr;
}

