#pragma once

void PatchMemory(ULONG_PTR Address, PBYTE Data, SIZE_T Size);
void PatchMemory_WithNOP(ULONG_PTR Address, SIZE_T Size);
void FixupFunction(ULONG_PTR Address, ULONG_PTR DestAddress);
const char* va(const char* string, ...);
const wchar_t* va(const wchar_t* string, ...);
std::string GetBinaryResource(int name);

bool FileExists(const char *pchFile);
bool isSubstr(std::string s, std::string sub);

#define DBG(x,...) OutputDebugStringA(va(x, __VA_ARGS__))

#pragma comment(lib, "Resource/Detours.lib")
#include "Resource/Typedef.h"
#include "Resource/Detours.h"