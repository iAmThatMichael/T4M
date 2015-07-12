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
	//DVAR: cg_fov
	//MODS: Clear cheat flag, set saved flag, set maximum to 90
	*(BYTE*)0x65EE46 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_SAVED;
	*(DWORD*)0x65EE31 = (DWORD)&cgFov90;
	
	//
}