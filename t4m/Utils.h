void PatchMemory(ULONG_PTR Address, PBYTE Data, SIZE_T Size);
void PatchMemory_WithNOP(ULONG_PTR Address, SIZE_T Size);
void FixupFunction(ULONG_PTR Address, ULONG_PTR DestAddress);
const char* va(const char* string, ...);
const wchar_t* va(const wchar_t* string, ...);
std::string GetBinaryResource(int name);

#pragma comment(lib, "Resource/Detours.lib")
#include "Resource/Typedef.h"
#include "Resource/Detours.h"
