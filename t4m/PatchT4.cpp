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

void PatchSteamDRM();
void PatchT4();
void PatchT4_AssetPool();
void PatchT4_Dvars();
void PatchT4_NoBorder();
void PatchT4_PreLoad();
void PatchT4_Version();

void Sys_RunInit()
{
	PatchT4();
}

void PatchSteamDRM()
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

void PatchT4()
{
	//PatchSteamDRM(); //NOTE: Do not enable this; at it's current state it breaks some stuff related to mod loading.
	//Side-note: Switching to "LanFixed" exe in order to continue development until said func is fixed.
	PatchT4_AssetPool();
	PatchT4_Dvars();
	PatchT4_NoBorder();
	PatchT4_PreLoad();
	PatchT4_Version();
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
	//ReallocateAssetPool(ASSET_TYPE_LOCALIZE, 9216);	// do not set, messed up pool.
	//ReallocateAssetPool(ASSET_TYPE_RAWFILE, 1024);	// not modified
	//ReallocateAssetPool(ASSET_TYPE_XANIM, 5100);
	//ReallocateAssetPool(ASSET_TYPE_XMODEL, 2000);
}

void PatchT4_PreLoad()
{
	nop(0x5FE685, 5);									// remove optimal settings popup
	*(BYTE*)0x5FF386 = (BYTE)0xEB;						// skip safe mode check
}

void PatchT4_Version()
{
	/*
	Useful addresses
	buildDateTime	0x8711E4
	buildGame		0x871228
	buildUser		0x871200
	buildNumber		0x59D35C (hex), PatchMemory doesn't like it :(
	buildNumber2	0x8D8D5C 0x59CECB why are there multiple instances (still more), none of these are correct. (end address +1 to not break the push opcode) Just replace in exe....
	consoleInfo		0x851208 (1.7.[buildNumber] T4 >)
	'7'				0x851200 (versionInfo)
	version check	0x5FD91B
	gameName		0x871228
	*/

	//PatchMemory(0xaddressoftjestring, yourstring, strlen(yourstring)+1);
	//DWORD version = 1337;
	//PatchMemory(0x59D35D, (PBYTE)&version, 4);
	//_patch((void *)0x59D35D, 1337, 4);				// note buildNumber2.
	//_patch((void *)0x59CECC, 1337, 4);				// ^^
	nop(0x5FD91B, 5);									// disable pc_newversionavailable check
	PatchMemory(0x851200, (PBYTE)"8", 2);
	PatchMemory(0x851208, (PBYTE)"%s.%s.%d %s > ", 14);
	PatchMemory(0x8711E4, (PBYTE)"1/1/1970", 9);		// date & time
	PatchMemory(0x871228, (PBYTE)"T4M", 4);				// game
	PatchMemory(0x871200, (PBYTE)"UNKNOWN", 8);			// user
	PatchMemory(0x8D8D5C, (PBYTE)"7331", 5);			// number2 (probably ID of user?)
}
