// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: Utility functions
//
// Initial author: UNKNOWN
// Started: 2015-07-08
// ==========================================================

#include "StdInc.h"
#include <sys/stat.h>

/*
va() variant by NTAuthority from http://tohjo.eu/citidev/citizenmp
*/
#define BUFFER_COUNT 8
#define BUFFER_LENGTH 32768

const char* va(const char* string, ...)
{
	static __thread int currentBuffer;
	static __thread char* buffer;

	if (!buffer)
	{
		buffer = new char[BUFFER_COUNT * BUFFER_LENGTH];
	}

	int thisBuffer = currentBuffer;

	va_list ap;
	va_start(ap, string);
	int length = vsnprintf(&buffer[thisBuffer * BUFFER_LENGTH], BUFFER_LENGTH, string, ap);
	va_end(ap);

	if (length >= BUFFER_LENGTH)
	{
		Com_Printf(0, "ERROR: Attempted to overrun string in call to va()!");
		exit(1);
	}

	buffer[(thisBuffer * BUFFER_LENGTH) + BUFFER_LENGTH - 1] = '\0';

	currentBuffer = (currentBuffer + 1) % BUFFER_COUNT;

	return &buffer[thisBuffer * BUFFER_LENGTH];
}

const wchar_t* va(const wchar_t* string, ...)
{
	static __thread int currentBuffer;
	static __thread wchar_t* buffer;

	if (!buffer)
	{
		buffer = new wchar_t[BUFFER_COUNT * BUFFER_LENGTH];
	}

	int thisBuffer = currentBuffer;

	va_list ap;
	va_start(ap, string);
	int length = vswprintf(&buffer[thisBuffer * BUFFER_LENGTH], BUFFER_LENGTH, string, ap);
	va_end(ap);

	if (length >= BUFFER_LENGTH)
	{
		Com_Printf(0, "ERROR: Attempted to overrun string in call to va()!");
		exit(1);
	}

	buffer[(thisBuffer * BUFFER_LENGTH) + BUFFER_LENGTH - 1] = '\0';

	currentBuffer = (currentBuffer + 1) % BUFFER_COUNT;

	return &buffer[thisBuffer * BUFFER_LENGTH];
}

std::string GetBinaryResource(int name)
{
	HMODULE handle;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)GetBinaryResource, &handle);

	HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL rcData = LoadResource(handle, rc);
	DWORD size = SizeofResource(handle, rc);

	std::string data((char*)LockResource(rcData), size);
	FreeResource(rcData);

	return data;
}

//code from Nukem/SE2Dev https://github.com/Nukem9/LinkerMod/blob/master/shared/utility.h 
void PatchMemory(ULONG_PTR Address, PBYTE Data, SIZE_T Size)
{
	DWORD d = 0;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

	for (SIZE_T i = 0; i < Size; i++)
		*(volatile BYTE *)(Address + i) = *Data++;

	VirtualProtect((LPVOID)Address, Size, d, &d);

	FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
}

void PatchMemory_WithNOP(ULONG_PTR Address, SIZE_T Size)
{
	DWORD d = 0;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

	for (SIZE_T i = 0; i < Size; i++)
		*(volatile BYTE *)(Address + i) = 0x90; //0x90 == opcode for NOP

	VirtualProtect((LPVOID)Address, Size, d, &d);

	FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
}

void FixupFunction(ULONG_PTR Address, ULONG_PTR DestAddress)
{
	DWORD data = (DestAddress - Address - 5);

	PatchMemory(Address + 0, (PBYTE)"\xE9", 1);
	PatchMemory(Address + 1, (PBYTE)&data, 4);
}

bool FileExists(const char* file)
{
	struct stat st;

	return (stat(file, &st) == 0);
}

bool isSubstr(std::string s, std::string sub) {
	return s.find(sub) != std::string::npos;
}