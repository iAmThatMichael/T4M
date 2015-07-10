#include "StdInc.h"

Com_Printf_t Com_Printf = (Com_Printf_t)0x59A2C0;
Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x59A170;

Dvar_RegisterBool_t Dvar_RegisterBool = (Dvar_RegisterBool_t)0x5EEE20;

typedef int(__cdecl * DB_GetXAssetSizeHandler_t)();
DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers = (DB_GetXAssetSizeHandler_t*)0x8DC708;

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
