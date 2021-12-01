#pragma once

#ifndef EXTERNALTOOLS_H
#define EXTERNALTOOLS_H

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <vector>
using namespace std;

DWORD GetProcID(const wchar_t * name);


uintptr_t  GetModuleBaseAddress(DWORD procID, const wchar_t * ModuleName);


int PrintModules(DWORD processID);

uintptr_t GetWriteAddress(HANDLE h, uintptr_t moduleBase, vector<UINT> offsets); //first offset must be base + offsets[i]

//ReadProcessMemory wrapper
template<typename T>
T GetValue(HANDLE handle, uintptr_t address)
{
	T buffer;
	ReadProcessMemory(handle, (BYTE*)address, &buffer, sizeof(buffer), 0);
	return buffer;
}

//WriteProcessMemory wrapper
template<typename T>
void WriteValue(HANDLE h, uintptr_t writeAddress, T val)
{
	WriteProcessMemory(h, (BYTE*)writeAddress, &val, sizeof(val), NULL);
}


#endif
