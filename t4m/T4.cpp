// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: World at War related functions
//
// Initial author: UNKNOWN
// Started: 2015-07-08
// ==========================================================

#include "StdInc.h"

Cbuf_AddText_t Cbuf_AddText = (Cbuf_AddText_t)0x594200;

Com_Printf_t Com_Printf = (Com_Printf_t)0x59A2C0;
Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x59A170;

// fucking __usercall
dvar_t* Dvar_RegisterBool(bool value, const char *dvarName, int flags, const char *description)
{
	DWORD func = 0x5EEE20;
	dvar_t* ret;
	__asm
	{
		push description
		push flags
		mov al, value
		mov edi, dvarName
		call func
		add esp, 8
		mov ret, eax
	}
	return ret;
}

typedef int(__cdecl * DB_GetXAssetSizeHandler_t)();
DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x8DCC18;

void** DB_XAssetPool = (void**)0x8DC828;
unsigned int* g_poolSize = (unsigned int*)0x8DC5D0;

void* ReallocateAssetPool(assetType_t type, unsigned int newSize)
{
	int elSize = DB_GetXAssetSizeHandlers[type]();
	void* poolEntry = malloc(newSize * elSize);
	DB_XAssetPool[type] = poolEntry;
	g_poolSize[type] = newSize;
	return poolEntry;
}
