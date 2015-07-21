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
	*(BYTE*)0x4781FE = 0xEB; // force enable ingame console

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
	PatchMemory(0x84B670, (PBYTE)"", 2); // set "." to blank since it spams it in dwLogOnComplete
	nop(0x5F9DF2, 5); // vv
	nop(0x5F9E2F, 5); // disable DebugReportProfileDVars call (was being spammed when in lobbies and such)
	nop(0x5A3C44, 5); // disable Com_Printf call for "ragdoll allocation failed" (causes frame drops up the ass)
	nop(0x57B15B, 5); // disable Com_Printf call for "g_numFriends is now %i" (internal)
	nop(0x57FE59, 5); // disable Com_Printf call for "nulling invite info for friend %s"
	nop(0x57FE15, 5); // disable Com_Printf call for "updating profile info for friend %s"
}