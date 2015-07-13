// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: World at War patches
//
// Initial author: UNKNOWN
// Started: 2015-07-08
// ==========================================================

#include "StdInc.h"

void PatchT4();
void PatchT4_AssetPool();
void PatchT4_Branding();
void PatchT4_Console();
void PatchT4_Dvars();
void PatchT4_NoBorder();
void PatchT4_PreLoad();
void PatchT4_SteamDRM();

void Sys_RunInit()
{
	PatchT4();
}

void PatchT4()
{
	PatchT4_SteamDRM();
	PatchT4_AssetPool();
	PatchT4_Console();
	PatchT4_Dvars();
	PatchT4_NoBorder();
	PatchT4_PreLoad();
	PatchT4_Branding();
}

void PatchT4_AssetPool()
{
	// increase pool sizes to similar (or greater) t5 sizes.
	ReallocateAssetPool(ASSET_TYPE_FX, 600);
	ReallocateAssetPool(ASSET_TYPE_IMAGE, 4096);
	ReallocateAssetPool(ASSET_TYPE_LOADED_SOUND, 2400);
	ReallocateAssetPool(ASSET_TYPE_MATERIAL, 4096);
	ReallocateAssetPool(ASSET_TYPE_STRINGTABLE, 80);
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 320);
	// unused pool allocations
	//ReallocateAssetPool(ASSET_TYPE_LOCALIZE, 9216); // do not set, messed up pool.
	//ReallocateAssetPool(ASSET_TYPE_RAWFILE, 1024); // not modified
	//ReallocateAssetPool(ASSET_TYPE_XANIM, 5100);
	//ReallocateAssetPool(ASSET_TYPE_XMODEL, 2000);
}

void PatchT4_PreLoad()
{
	nop(0x5FE685, 5); // remove optimal settings popup
	*(BYTE*)0x5FF386 = (BYTE)0xEB; // skip safe mode check
}

void PatchT4_SteamDRM()
{
	// Replace encrypted .text segment
	DWORD size = 0x3EA000;
	std::string data = GetBinaryResource(IDB_TEXT);
	uncompress((unsigned char*)0x401000, &size, (unsigned char*)data.data(), data.size());

	// Apply new entry point
	HMODULE hModule = GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);
	ntHeader->OptionalHeader.AddressOfEntryPoint = 0x3AF316;
}