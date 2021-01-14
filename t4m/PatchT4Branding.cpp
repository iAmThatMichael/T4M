// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: branding for T4M
//
// Initial author: DidUknowiPwn
// Started: 2015-07-12
// ==========================================================

#include "StdInc.h"

const char* SetConsoleVersion();
const char* SetShortVersion();

void PatchT4_Branding()
{
	// TODO: Replace shortversion DVars and other version related locations
	nop(0x59D68B, 5);										// don't play intro video
	nop(0x5FD91B, 5);										// disable pc_newversionavailable check
	PatchMemory(0x851208, (PBYTE)CONSOLEVERSION_STR, 14);	// change the console input version
	PatchMemory(0x871EE8, (PBYTE)va("T4-SP (r%i)\n", VERSION), 32);
	Detours::X86::DetourFunction((PBYTE)0x5B5A20, (PBYTE)&SetShortVersion, Detours::X86Option::USE_CALL); // change version number bottom right of main
	Detours::X86::DetourFunction((PBYTE)0x4743D2, (PBYTE)&SetConsoleVersion, Detours::X86Option::USE_CALL); // change the version info of console window
	Detours::X86::DetourFunction((PBYTE)0x59D393, (PBYTE)&SetConsoleVersion, Detours::X86Option::USE_CALL); // change the version info of version 
}

const char* SetConsoleVersion()
{
	return va("Call of Duty %s", VERSION_STR);
}

const char* SetShortVersion()
{
	return va(SHORTVERSION_STR);
}