// ==========================================================
// project 'secretSchemes'
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Manages the initialization of t4cli.
//
// Initial author: NTAuthority
// Adapated: 2015-07-08
// Started: 2011-05-04
// ==========================================================

#include "StdInc.h"

void Sys_RunInit();

static BYTE originalCode[5];
static PBYTE originalEP = 0;

void Main_UnprotectModule(HMODULE hModule)
{
	PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

	// unprotect the entire PE image
	SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
	DWORD oldProtect;
	VirtualProtect((LPVOID)hModule, size, PAGE_EXECUTE_READWRITE, &oldProtect);
}

void Main_DoInit()
{
	// return to the original EP
	memcpy(originalEP, &originalCode, sizeof(originalCode));

	// unprotect our entire PE image
	HMODULE hModule;
	if (SUCCEEDED(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)Main_DoInit, &hModule)))
	{
		Main_UnprotectModule(hModule);
	}

	Sys_RunInit();

	hModule = GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

	// back up original code
	originalEP = (PBYTE)((DWORD)hModule + ntHeader->OptionalHeader.AddressOfEntryPoint);

	__asm jmp originalEP
}

void Main_SetSafeInit()
{
	// find the entry point for the executable process, set page access, and replace the EP
	HMODULE hModule = GetModuleHandle(NULL); // passing NULL should be safe even with the loader lock being held (according to ReactOS ldr.c)

	if (hModule)
	{
		PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
		PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

		Main_UnprotectModule(hModule);

		// back up original code
		PBYTE ep = (PBYTE)((DWORD)hModule + ntHeader->OptionalHeader.AddressOfEntryPoint);
		memcpy(originalCode, ep, sizeof(originalCode));

		// patch to call our EP
		int newEP = (int)Main_DoInit - ((int)ep + 5);
		ep[0] = 0xE9; // for some reason this doesn't work properly when run under the debugger
		memcpy(&ep[1], &newEP, 4);

		originalEP = ep;
	}
}

//extern "C" void __declspec(dllimport) DependencyFunctionCCAPI();

bool __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (*(DWORD*)0x401000 == 0x9EF490B8 || *(DWORD*)0x401000 == 0x83EC8B55) // SP! (LanFixed | Steam)
			Main_SetSafeInit();
		//else
		//	MessageBoxA(0, "Incompatiable Call of Duty World at War exe.\nT4M will not be loaded.", "Error", MB_OK);
	}
	return true;
}
