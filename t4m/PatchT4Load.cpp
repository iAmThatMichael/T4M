// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: various loading changes for T4M
//
// Initial author: DidUknowiPwn
// Started: 2015-10-06
// ==========================================================

#include "StdInc.h"
#include "T4.h"

dvar_t** fs_localAppData = (dvar_t**)0x2122AF0;
dvar_t** fs_game = (dvar_t**)0x2122B00;
dvar_t** fs_basepath = (dvar_t**)0x02123C14;
dvar_t** dedicated = (dvar_t**)0x0212B2F4;

void makeFFLoadStruct(XZoneInfo *zoneInfo, int zoneCount, int sync)
{
	static XZoneInfo *newZoneInfo;

	newZoneInfo = new XZoneInfo[zoneCount];
	memset(newZoneInfo, 0, sizeof(XZoneInfo) * zoneCount);

	// continue this when better understood
}

void __cdecl ModFFLoadHook(XZoneInfo *zoneInfo, int zoneCount, int sync)
{
	static XZoneInfo *modZoneInfo;
	// in t4 mods are loaded from appdata not base game
	if (FileExists(va("%s\\%s\\mod_ex.ff", (*fs_localAppData)->current.string, (*fs_game)->current.string)))
	{
		modZoneInfo = new XZoneInfo[zoneCount];
		memset(modZoneInfo, 0, sizeof(XZoneInfo) * (zoneCount)); // is needed, causes mod_ex to freeze w/o it 
		for (int i = 0; i < zoneCount; ++i)
		{
			modZoneInfo[i].name = zoneInfo[i].name;
			modZoneInfo[i].allocFlags = zoneInfo[i].allocFlags;
			modZoneInfo[i].freeFlags = zoneInfo[i].freeFlags;
		}
		// if game freezes on startup mod_ex might be bad.
		modZoneInfo[zoneCount].name = "mod_ex";
		modZoneInfo[zoneCount].allocFlags = 2048; // allocFlags indicate what loading method to go through, 2048 matches code_post_gfx/mod from 006D5672
		modZoneInfo[zoneCount].freeFlags = 0;

		DB_LoadXAssets(modZoneInfo, zoneCount + 1, sync); // +1 to count to load mod_ex
	}
	else
	{
		DB_LoadXAssets(zoneInfo, zoneCount, sync);
	}
}

void __cdecl FFLoadHook(XZoneInfo *zoneInfo, int zoneCount, int sync)
{
	static XZoneInfo *ffZoneInfo;
	// in t4 mods are loaded from appdata not base game
	if (FileExists(va("%s\\zone\\t4m_patch.ff", (*fs_basepath)->current.string)))
	{
		ffZoneInfo = new XZoneInfo[zoneCount];
		memset(ffZoneInfo, 0, sizeof(XZoneInfo) * (zoneCount)); // is needed, causes mod_ex to freeze w/o it 
		for (int i = 0; i < zoneCount; ++i)
		{
			ffZoneInfo[i].name = zoneInfo[i].name;
			ffZoneInfo[i].allocFlags = zoneInfo[i].allocFlags;
			ffZoneInfo[i].freeFlags = zoneInfo[i].freeFlags;
		}
		// if game freezes on startup mod_ex might be bad.
		ffZoneInfo[zoneCount].name = "t4m_patch";
		ffZoneInfo[zoneCount].allocFlags = 2048; // allocFlags indicate what loading method to go through, 2048 matches code_post_gfx/mod from 006D5672
		ffZoneInfo[zoneCount].freeFlags = 0;

		DB_LoadXAssets(ffZoneInfo, zoneCount + 1, sync); // +1 to count
	}
	else
	{
		DB_LoadXAssets(zoneInfo, zoneCount, sync);
	}
}

void PatchT4_Load()
{
	// to be used?
	//Detours::X86::DetourFunction((PBYTE)0x006D5728, (PBYTE)&FFLoadHook, Detours::X86Option::USE_CALL);
	Detours::X86::DetourFunction((PBYTE)0x006D5672, (PBYTE)&ModFFLoadHook, Detours::X86Option::USE_CALL);
	//00644C5D, r_init
	//if ((*dedicated)->current.integer > 0)
	//nop(0x00644C5D, 5);
}