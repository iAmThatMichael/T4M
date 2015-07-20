// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: messing with filesystem stuff
//
// Initial author: TheApadayo
//
// Started: 2015-07-18
// ==========================================================
#include "StdInc.h"

DWORD fsopen = 0x7AC6ED;

std::map<FILE*, std::string> openFiles1;
std::map<std::string, FILE*> openFiles2;

FILE* fopen_hook(const char* file, const char* mode)
{
	if (openFiles2.find(std::string(file)) != openFiles2.end())
	{
		Com_Printf(0, "File '%s' is already open! Returning previous handle\n", file);
		return openFiles2[std::string(file)];
	}

	FILE* fp;
	__asm
	{
		push mode
		push file
		call fsopen
		add esp, 8
		mov fp, eax
	}
	if (!fp) return NULL;

	openFiles1[fp] = std::string(file);
	openFiles2[std::string(file)] = fp;
	Com_Printf(0, "File '%s' opened\n", file);
	return fp;
}

DWORD fclose_nolock = 0x7ACF6E;

void* fclose_hook(FILE* f)
{
	void* r;
	__asm
	{
		push f
		call fclose_nolock
		add esp, 4
		mov r, eax
	}

	if (openFiles1.find(f) != openFiles1.end())
	{
		std::string s = openFiles1[f];
		Com_Printf(0, "File '%s' closed\n", openFiles1[f].c_str());
		openFiles2.erase(openFiles2.find(openFiles1[f]));
		openFiles1.erase(openFiles1.find(f));
		return r;
	}

	Com_Printf(0, "Got file we don't know about!\n");
	return r;
}

void PatchT4_FileDebug()
{
	call(0x7AC7B1, fopen_hook, PATCH_JUMP);
	call(0x7AD03A, fclose_hook, PATCH_CALL);
}