#include "StdInc.h"
#include "T4.h"
#include <ctime>

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

void Sys_RunInit()
{
	PatchSteamDRM();

	/*
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
	PatchMemory_WithNOP(0x5FD91B, 5);					// disable pc_newversionavailable check
	PatchMemory(0x851200, (PBYTE)"8", 2);
	PatchMemory(0x851208, (PBYTE)"%s.%s.%d %s >", 14);
	PatchMemory(0x8711E4, (PBYTE)"1/1/1970", 9);		// date & time
	PatchMemory(0x871228, (PBYTE)"T4M", 4);				// game
	PatchMemory(0x871200, (PBYTE)"UNKNOWN", 8);			// user
	PatchMemory(0x8D8D5C, (PBYTE)"7331", 5);			// number2 (probably ID of user?)

	// NOTE: some assets will work and some will crash, example everything
	// in here will crash but fx.
	//
	// doubling asset pool sizes
	ReallocateAssetPool(ASSET_TYPE_FX, 800);
	ReallocateAssetPool(ASSET_TYPE_IMAGE, 4800);
	ReallocateAssetPool(ASSET_TYPE_LOADED_SOUND, 3200);
	ReallocateAssetPool(ASSET_TYPE_MATERIAL, 4096);
	ReallocateAssetPool(ASSET_TYPE_WEAPON, 512); 
	ReallocateAssetPool(ASSET_TYPE_XMODEL, 2000);
}

