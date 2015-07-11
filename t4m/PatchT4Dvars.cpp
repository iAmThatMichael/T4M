// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: Dvar modifications, from changing flags to 
//          restricting changes
//
// Initial author: Pigophone / NTAuthority (copied from 
//                 PatchMW2.cpp)
// Adapated: 2015-07-10
// Started: 2012-04-19
// ==========================================================

#include "StdInc.h"

float cgFov90 = 90.0f;

void PatchT4_Dvars()
{
	*(BYTE*)0x65EE46 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_SAVED; // un-protected cg_fov and add saved flag

	//*(DWORD*)0x65EE60 = (DWORD)&cgFov90; // find right address
}