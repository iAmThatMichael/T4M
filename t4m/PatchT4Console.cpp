// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: Changes/functionality related to console.
//
// Initial author: DidUknowiPwn
// Started: 2015-07-12
// ==========================================================

#include "StdInc.h"
#include "T4.h"

dvar_t* con_external;

vec4_t whiteColor = { 8.0f, 8.0f, 8.0f, 1.0f };

typedef void(__cdecl * ConDrawInput_Text_t)(const char* text, vec4_t* color);
ConDrawInput_Text_t ConDrawInput_Text = (ConDrawInput_Text_t)0x471BC0;

typedef void(__cdecl * ConDrawInput_TextLimitChars_t)(const char* text, int numChars, vec4_t* color);
ConDrawInput_TextLimitChars_t ConDrawInput_TextLimitChars = (ConDrawInput_TextLimitChars_t)0x471CB0;

// Dvar_RegisterInt(int default, const char* name, int min, int max, int flags, const char* description); 0x5EEEA0

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

void ShowExternalConsole()
{
	DWORD Sys_ShowConsole_f = 0x006057F0;
	DWORD sub_5E3CA0 = 0x005E3CA0;

	__asm call sub_5E3CA0

	if (con_external->current.value)
		__asm call Sys_ShowConsole_f
}

void FilterConsoleSpam()
{
	nop(0x57C40F, 5); // disable Com_Printf call for "."
	nop(0x57C51B, 5); // ^^
	nop(0x57C82B, 5); // ^^
	nop(0x57CB94, 5); // ^^
	// --- had to split this apart since the "." was used for other parts of code, i.e. was causing the "." to show in mods list
	nop(0x5F9DF2, 5); // disable DebugReportProfileDVars call
	nop(0x5F9E2F, 5); // ^^
	nop(0x5A3C44, 5); // disable Com_Printf call for "ragdoll allocation failed"
	nop(0x57B15B, 5); // disable Com_Printf call for "g_numFriends is now %i" (internal)
	nop(0x57FE59, 5); // disable Com_Printf call for "nulling invite info for friend %s"
	nop(0x57FE15, 5); // disable Com_Printf call for "updating profile info for friend %s"
	nop(0x5FC93D, 5); // disable Com_Printf call for "Failed to log on."
	nop(0x5FC9CD, 5); // ^^
	nop(0x59CF04, 5); // disable Com_Printf call for build version in Com_Init_Try_Block_Function
	nop(0x005FC35B, 5); // disable Com_Printf call for "Upload Bandwidth:~"
	nop(0x005FC382, 5); // disable Com_Printf call for "Download Bandwidth:~"
	nop(0x005EDA8C, 5); // disable Com_sprintf call for "dvar set"
	nop(0x005EDA9B, 5); // disable Com_PrintMessage call for "dvar set"
}

void PatchT4_ExternalConsole()
{
	Detours::X86::DetourFunction((PBYTE)0x0059D0F3, (PBYTE)&ShowExternalConsole, Detours::X86Option::USE_CALL);
}

void PatchT4_ConsoleBox()
{
	// call our functionality to draw another line
	call(0x47294C, drawDetailedDvarMatchStub, PATCH_CALL);
	*(BYTE*)0x4727DD = 3; // increase line number for box
}

void testCmd_f()
{
	Com_Printf(0, "Surprise motherfucker. You have: ^3%i ^7args passed\n", Cmd_Argc() - 1);
}

void __cdecl DB_ListAssetPool_f()
{
	char *v0; // eax@4
	char *v1; // eax@6
	XAssetType type; // ST24_4@6
	signed int i; // [sp+10h] [bp-Ch]@2

	unsigned int* g_poolSize = (unsigned int*)0x8DC5D0;

	if (Cmd_Argc() >= 2)
	{
		v1 = Cmd_Argv(1);
		type = (XAssetType)atoi(v1);
		DB_ListAssetPool(type, false);
	}
	else
	{
		Com_Printf(0, "listassetpool <poolnumber>: lists all the assets in the specified pool\n");
		for (i = 0; i < ASSET_TYPE_MAX; ++i)
		{
			v0 = DB_GetXAssetTypeName(i);
			Com_Printf(0, "%d %s %i\n", i, v0, g_poolSize[i]);
		}
	}
}

void DB_ListAssetCounts_f()
{
	Com_Printf(0, "Listing assets in all pools.\n");

	for (int i = 0; i < ASSET_TYPE_MAX; ++i)
	{
		DB_ListAssetPool((XAssetType)i, true);
	}
}

void Cmd_Init_T4()
{
	DWORD Cmd_Init_T4 = 0x00595200;

	__asm call Cmd_Init_T4

	//Cmd_AddCommand("testcmd", testCmd_f);
	Cmd_AddCommand("listassetpool", DB_ListAssetPool_f);
	Cmd_AddCommand("listassetcounts", DB_ListAssetCounts_f);
}

void ShitTest()
{
	for (int i = 0; i < ASSET_TYPE_MAX; ++i)
	{
		DB_ListAssetPool((XAssetType)i, true);
	}
}


void PatchT4_ConsoleCommands()
{
	Detours::X86::DetourFunction((PBYTE)0x0059CF48, (PBYTE)&Cmd_Init_T4, Detours::X86Option::USE_CALL);
	//Detours::X86::DetourFunction((PBYTE)0x00608D16, (PBYTE)&ShitTest, Detours::X86Option::USE_CALL);
}

const char* Draw_G_Ents()
{
	int entityCount = *(WORD*)0x018F5D94; // or known as numGEntities or max_ents, it's the highest amount of entities loaded
	gentity_s* freeEntity = *(gentity_s**)0x18f5d98; // or known as the 'e' from t6r
	const char* s;

	while (freeEntity)
	{
		entityCount--;
		freeEntity = (gentity_s*)freeEntity->nextFree;
	}

	s = va("%i/1023", entityCount + 1); // current / max (is 1023 as defined by max_ents/numGEntities in G_Spawn

	return s;	
}

void PatchT4_GetGEnts()
{
	// override the lvl free msg
	PatchMemory(0x0084D5A0, (PBYTE)" total ents", 11);
	// detour the va call
	Detours::X86::DetourFunction((PBYTE)0x00439C2D, (PBYTE)&Draw_G_Ents, Detours::X86Option::USE_CALL);
	// remove verbose from cg_drawfps array, ends due to null terminator
	*(DWORD *)0x8CFCE8 = 0;
	// change jl to jmp, never executes cg_drawfps 3
	PatchMemory(0x00439C89, (PBYTE)"\xE9\x12\x03\x00\x00", 5);
}

void PatchT4_Console()
{
	con_external = Dvar_RegisterBool(0, "con_external", DVAR_FLAG_ARCHIVE, "Enable the external console (requires restart).");

	*(BYTE*)0x4781FE = 0xEB; // force enable ingame console

	FilterConsoleSpam();

	PatchT4_GetGEnts();
	PatchT4_ConsoleCommands();
	PatchT4_ConsoleBox();
	PatchT4_ExternalConsole();
}