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

static char* fs_basegame = "+set fs_basegame " FS_BASEGAME;
DWORD sys_cmdLine = 0x5FF6F8;
const char* SetConsoleVersion();
const char* SetShortVersion();

void PatchT4_Branding()
{
	/*
	Useful addresses
	buildDateTime	0x8711E4
	buildGame		0x871228
	buildUser		0x871200
	buildNumber		0x59D35C
	buildNumber2	0x8D8D5C
	consoleInfo		0x851208
	'7'				0x851200
	'1'				0x851204
	version check	0x5FD91B
	gameName		0x871228
					0x4E55B20
	*/
	// TODO: Replace shortversion DVars and other version related locations
	nop(0x59D68B, 5); // don't play intro video
	nop(0x5FD91B, 5);										// disable pc_newversionavailable check
	PatchMemory(0x851208, (PBYTE)CONSOLEVERSION_STR, 14);	// change the console input version
	//PatchMemory(0x851204, (PBYTE)"2", 2); // a1 for shortversion, replaces r_fullscreen default
	//PatchMemory(0x851200, (PBYTE)"0", 2); // a2
	Detours::X86::DetourFunction((PBYTE)0x5B5A20, (PBYTE)&SetShortVersion, Detours::X86Option::USE_CALL); // change version number bottom right of main
	Detours::X86::DetourFunction((PBYTE)0x4743D2, (PBYTE)&SetConsoleVersion, Detours::X86Option::USE_CALL); // change the version info of console window
	Detours::X86::DetourFunction((PBYTE)0x59D393, (PBYTE)&SetConsoleVersion, Detours::X86Option::USE_CALL); // change the version info of version 

	//PatchMemory(sys_cmdline, (PBYTE)&fs_basegame, 4); // add fs_basegame to sys_cmdline
}

const char* SetConsoleVersion()
{
	return va("Call of Duty %s", VERSION_STR);
}

const char* SetShortVersion()
{
	return va(SHORTVERSION_STR);
}