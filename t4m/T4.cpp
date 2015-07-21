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

extern "C"
{
	//Cbuf_AddText_t Cbuf_AddText = (Cbuf_AddText_t)0x594200;

	//Com_Printf_t Com_Printf = (Com_Printf_t)0x59A2C0;
	Com_PrintMessage_t Com_PrintMessage = (Com_PrintMessage_t)0x59A170;
}

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

// fucking __usercall
// somehow is still broken, works on and off
void Cbuf_AddText(const char* text, int localClientNum)
{
	DWORD func = 0x594200;
	__asm
	{
		mov eax, text
		mov ecx, localClientNum
		call func
	}
}

void Com_Printf(int channel, const char* format, ...)
{
	static char buffer[32768] = { 0 };

	va_list va;
	va_start(va, format);

	//vprintf(format, va);
	_vsnprintf(buffer, sizeof(buffer), format, va);
	va_end(va);

	Com_PrintMessage(channel, buffer, 0);
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
