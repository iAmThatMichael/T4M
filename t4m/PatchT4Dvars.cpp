// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: Dvar modifications, from changing flags to 
//          restricting changes
//
// Initial author: Pigophone / NTAuthority (copied from 
//                 PatchMW2.cpp)
// Adapated: 2015-07-10
// Started: 2012-04-19
// ==========================================================

#include "StdInc.h"

float cgFov90 = 90.0f;

vec4_t whiteColor = { 8.0f, 8.0f, 8.0f, 1.0f };

typedef void(__cdecl * ConDrawInput_Text_t)(const char* text, vec4_t* color);
ConDrawInput_Text_t ConDrawInput_Text = (ConDrawInput_Text_t)0x471BC0;

typedef void(__cdecl * ConDrawInput_TextLimitChars_t)(const char* text, int numChars, vec4_t* color);
ConDrawInput_TextLimitChars_t ConDrawInput_TextLimitChars = (ConDrawInput_TextLimitChars_t)0x471CB0;

struct ConDrawInputGlob
{
	char autoCompleteChoice[64];
	int matchIndex;
	int matchCount;
	const char *inputText;
	int inputTextLen;
	bool hasExactMatch;
	bool mayAutoComplete;
	float x;
	float y;
	float leftX;
	float fontHeight;
};

ConDrawInputGlob* conDraw = (ConDrawInputGlob*)0x92C370;

void DrawDvarFlags(dvar_t* dvar)
{
	__int16 flags = dvar->flags;

	const char* flagsString = va("Flags: %s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
		(flags & DVAR_FLAG_ARCHIVE ? "Archive, " : ""),
		(flags & DVAR_FLAG_USERINFO ? "UserInfo, " : ""),
		(flags & DVAR_FLAG_SERVERINFO ? "ServerInfo, " : ""),
		(flags & DVAR_FLAG_SYSTEMINFO ? "SystemInfo, " : ""),
		(flags & DVAR_FLAG_INIT ? "Init, " : ""),
		(flags & DVAR_FLAG_LATCH ? "Latch, " : ""),
		(flags & DVAR_FLAG_ROM ? "Rom, " : ""),
		(flags & DVAR_FLAG_CHEAT ? "Cheat, " : ""),
		(flags & DVAR_FLAG_DEVELOPER ? "Developer, " : ""),
		(flags & DVAR_FLAG_SAVED ? "Saved, " : ""),
		(flags & DVAR_FLAG_NORESTART ? "NoRestart, " : ""),
		(flags & DVAR_FLAG_CHANGEABLE_RESET ? "ChangeableReset, " : ""),
		(flags & DVAR_FLAG_EXTERNAL ? "External, " : ""),
		(flags & DVAR_FLAG_AUTOEXEC ? "AutoExec" : ""));

	// increase by one line and reset to left side
	conDraw->y += conDraw->fontHeight;
	conDraw->x = conDraw->leftX;

	ConDrawInput_TextLimitChars(flagsString, 40, &whiteColor);
}

void __declspec(naked) drawDetailedDvarMatchStub()
{
	__asm
	{
		push [esp + 12]
		push [esp + 12]
		push [esp + 12]
		call ConDrawInput_TextLimitChars

		push edi
		call DrawDvarFlags
		add esp, 16

		retn
	}
}

void Patch_ConsoleBox()
{
	// call our functionality to draw another line
	call(0x47294C, drawDetailedDvarMatchStub, PATCH_CALL);

	*(BYTE*)0x4727DD = 3; // increase line number for box
}

void PatchT4_Dvars()
{
	//DVAR: cg_fov
	//MODS: Clear cheat flag, set arhive flag, set maximum to 90
	*(WORD*)0x65EE46 ^= DVAR_FLAG_CHEAT | DVAR_FLAG_ARCHIVE;
	*(float**)0x65EE31 = &cgFov90;

	//DVAR: cg_fovScale
	//MODS: Clear cheat flag, set arhive flag
	*(WORD*)0x65EE7C ^= DVAR_FLAG_CHEAT | DVAR_FLAG_ARCHIVE;

	Patch_ConsoleBox(); // do cusom drawing stuff for dvars in console
}