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
	//MODS: Clear cheat flag, set arhive flag, set maximum to 90
	*(WORD*)0x65EE46 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_ARCHIVE;
	*(float**)0x65EE31 = &cgFov90;

	//DVAR: cg_fovMin
	//MODS: Clear cheat flag, set arhive flag
	*(WORD*)0x65EEB6 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_ARCHIVE;

	//DVAR: cg_fovScale
	//MODS: Clear cheat flag, set arhive flag
	*(WORD*)0x65EE7C ^= DVAR_FLAG_CHEAT | DVAR_FLAG_ARCHIVE;

	//DVAR: fs_basegame
	//MODS: Clear cheat flag, set archive flag
	//*(WORD*)0x005DDEDD ^= DVAR_FLAG_INIT;
}