// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: Changes/functionality related to console.
//
// Initial author: DidUknowiPwn
// Started: 2015-07-12
// ==========================================================

#include "StdInc.h"

void FilterConsoleSpam();

void PatchT4_Console()
{
	Sys_ShowConsole();
	FilterConsoleSpam();
}


void Sys_ShowConsole()
{
	DWORD func = 0x6057F0;
	__asm
	{
		call func
	}
}


void FilterConsoleSpam()
{
	nop(0x57B15B, 5); // disable Com_Printf call for g_numFriends is now %i (internal)
	nop(0x57FE59, 5); // disable Com_Printf call for "nulling invite info for friend %s"
	PatchMemory(0x86F8E4, (PBYTE)"", 36); // replace updating profile info for friend %s with blanks, noping the address causes a crash elsewhere.
}