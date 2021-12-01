// Civ5Hack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <vector>
#include "externaltools.h"

using namespace std; 


int main()
{
	DWORD procID = GetProcID(L"CivilizationV.exe");
	uintptr_t moduleBase = GetModuleBaseAddress(procID, L"CvGameCore_Expansion2.dll");
	HANDLE h = NULL;
	h = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);
	uintptr_t goldWriteAddress = NULL; 
	uintptr_t cultureWriteAddress = NULL; 
	uintptr_t faithWriteAddress = NULL;
	uintptr_t goldenAgeWriteAddress = NULL;
	uintptr_t happinessWriteAddress = NULL;
	uintptr_t scienceWriteAddress = NULL;
	const vector<UINT> goldOffsets = { 0x3de9dc, 0xf528, 0x8 };
	const vector<UINT> cultureOffsets = { 0x3de9dc, 0xa4 };
	const vector<UINT> faithOffsets = { 0x3de9dc, 0xdc };
	const vector<UINT> goldenAgeOffsets = { 0x3de9dc, 0x204 };
	const vector<UINT> happinessOffsets = { 0x3de9dc, 0xe8 };
	const vector<UINT> scienceOffsets = { 0x3dffb0, 0xba0, 0xC, 0x0 };
	const vector<UINT> scienceIndexOffsets = { 0x3de9dc, 0x10e0, 0x2c, 0xff4, 0x4 }; //can't use static write address, the location of index changes during runtime 
	cultureWriteAddress = GetWriteAddress(h, moduleBase, cultureOffsets);
	goldWriteAddress = GetWriteAddress(h, moduleBase, goldOffsets);
	faithWriteAddress = GetWriteAddress(h, moduleBase, faithOffsets);
	goldenAgeWriteAddress = GetWriteAddress(h, moduleBase, goldenAgeOffsets);
	happinessWriteAddress = GetWriteAddress(h, moduleBase, happinessOffsets);
	scienceWriteAddress = GetWriteAddress(h, moduleBase, scienceOffsets);
	char input = 0;
	int write = 0;
	while (input != 'Q')
	{
		cout << "-------------CURRENT VALUES ------------" << endl;
		cout << "Current gold value: " << GetValue<int>(h, goldWriteAddress) / 100 << endl;
		cout << "Current culture value: " << GetValue<int>(h, cultureWriteAddress) << endl;
		cout << "Current faith value: " << GetValue<int>(h, faithWriteAddress) << endl;
		cout << "Current golden age value: " << GetValue<int>(h, goldenAgeWriteAddress) << endl;
		cout << "Current happiness: " << GetValue<int>(h, happinessWriteAddress) << endl;
		cout << "Current science: " << GetValue<int>(h, scienceWriteAddress + (GetValue<int>(h, GetWriteAddress(h, moduleBase, scienceIndexOffsets) ) * 4)) / 100 << endl;
		cout << "----------------------------------------" << endl; 
		cout << "-------------OPTIONS--------------------" << endl; 
		cout << "Gold : G" << endl; 
		cout << "Culture : C" << endl;
		cout << "Faith : F" << endl; 
		cout << "Golden Age : A" << endl; 
		cout << "Happiness : H" << endl;
		cout << "Science : S" << endl;
		cout << "Update : U" << endl; 
		cout << "Quit : Q" << endl; 
		cout << "----------------------------------------" << endl;
		cout << "Pick Option: "; 
		cin >> input;
		if (input == 'Q')
		{
			return 0;
		}
		if (input == 'U')
		{
			cout << endl << endl;
			continue; 
		}
		cout << "Enter value to write: ";
		cin >> write; 
		if (input == 'G')
		{
			write = write * 100; 
			WriteValue<int>(h, goldWriteAddress, write);
		}
		if (input == 'C')
		{
			WriteValue<int>(h, cultureWriteAddress, write);
		}
		if (input == 'F')
		{
			WriteValue<int>(h, faithWriteAddress, write);
		}
		if (input == 'A')
		{
			WriteValue<int>(h, goldenAgeWriteAddress, write);
		}
		if (input == 'H')
		{
			WriteValue<int>(h, happinessWriteAddress, write);
		}
		if (input == 'S')
		{
			write = write * 100;
			WriteValue<int>(h, scienceWriteAddress + (GetValue<int>(h, GetWriteAddress(h, moduleBase, scienceIndexOffsets)) * 4), write);
		}
		cout << endl << endl;
	}
    return 0;
}

