// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: Increasing memory pool sizes
//
// Initial author: TheApadayo
//
// Started: 2015-07-18
// ==========================================================

#include "StdInc.h"

void PatchT4_MemoryLimits()
{
	// increase pool sizes to similar (or greater) t5 sizes.
	ReallocateAssetPool(ASSET_TYPE_FX, 600);
	ReallocateAssetPool(ASSET_TYPE_IMAGE, 4096);
	ReallocateAssetPool(ASSET_TYPE_LOADED_SOUND, 2400);
	ReallocateAssetPool(ASSET_TYPE_MATERIAL, 4096);
	ReallocateAssetPool(ASSET_TYPE_STRINGTABLE, 80);
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 320);


	// change the size of g_mem from 0x12C00000 to 0x14800000 (what it is in t5)
	*(DWORD*)0x5F5492 = 0x14800000;
	*(DWORD*)0x5F54D1 = 0x14800000;
	*(DWORD*)0x5F54DB = 0x14800000;
}