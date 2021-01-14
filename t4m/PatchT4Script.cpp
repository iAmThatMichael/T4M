// ==========================================================
// alterIWnet project
// 
// Component: aiw_client
// Sub-component: steam_api
// Purpose: Functionality to interact with the GameScript 
//          runtime.
//
// Initial author: NTAuthority
// Adapted: 2015-07-21
// Started: 2011-12-19
// ==========================================================

#include "StdInc.h"
#include "T4.h"

int __cdecl Scr_GetNumParam(scriptInstance_t inst);

dvar_t** developer = (dvar_t**)0x01F55288;
dvar_t** developer_script = (dvar_t**)0x01F9646C;
dvar_t** logfile = (dvar_t**)0x01F552BC;

dvar_t* developer_funcdump;

// custom functions
typedef struct
{
	const char* functionName;
	scr_function_t functionCall;
	int developerOnly;
} scr_funcdef_t;


#pragma region setupFunctions
static std::map<std::string, scr_funcdef_t> scriptFunctions;

scr_function_t Scr_GetCustomFunction(const char** name, int* isDeveloper)
{
	scr_funcdef_t func = scriptFunctions[*name];

	if (func.functionName)
	{
		*name = func.functionName;
		*isDeveloper = func.developerOnly;

		return func.functionCall;
	}
	else
		return NULL;
}


void Scr_DeclareFunction(const char* name, scr_function_t func, bool developerOnly = false)
{
	scr_funcdef_t funcDef;
	funcDef.functionName = name;
	funcDef.functionCall = func;
	funcDef.developerOnly = (developerOnly) ? 1 : 0;

	scriptFunctions[name] = funcDef;
}
#pragma endregion setupFunctions

#pragma region engineFunctions
int __cdecl Scr_GetNumParam(scriptInstance_t inst)
{
	DWORD *value = (DWORD *)0x03BD471C; // getNumParamArray location
	return value[4298 * inst];
}

RefString *__cdecl GetRefString(scriptInstance_t inst, unsigned int stringValue)
{
	DWORD *gScrMemTreePub = (DWORD *)0x03702390;
	return (RefString *)&(&gScrMemTreePub)[3 * stringValue + 1];
}

char *__cdecl SL_ConvertToString(unsigned int stringValue, scriptInstance_t inst)
{
	char *v3;
	if (stringValue)
		v3 = GetRefString(inst, stringValue)->str;
	else
		v3 = 0;
	return v3;
}

void Scr_ClearOutParams(scriptInstance_t v1)
{
	static DWORD dwCall = 0x00693DA0;

	__asm
	{
		mov edi, v1
		call[dwCall]
	}
}

void __cdecl IncInParam(scriptInstance_t inst)
{
	Scr_ClearOutParams(inst);

	// TO-DO: define sys_error
	//if (dword_A05AC98[4298 * inst] == dword_A05AC8C[4298 * inst])
	//	Sys_Error("Internal script stack overflow");
	((DWORD *)0xA05AC98)[4296 * inst] += 8;
	++((DWORD *)0xA05ACA0)[4296 * inst];
}

void __cdecl Scr_AddInt(int value, scriptInstance_t inst)
{
	IncInParam(inst);
	*(DWORD *)((((DWORD *)0x03BD4710)[4296 * value]) + 4) = 6;
	*(DWORD *)(((DWORD *)0x03BD4710)[4296 * value]) = value;
}
#pragma endregion engineFunctions

#pragma region engineHKFunctions
void(__cdecl *__cdecl Scr_GetFunction_Hook(const char **pName, int *type))()
{
	// this is aids and I don't care
	// also if running debugger and a customf func is executed the debugger will instadie
	void(__cdecl *function)();
	// check if the function passed is part of our custom funcs
	if (!(scriptFunctions.find(std::string(*pName)) != scriptFunctions.end()))
		function = (void(__cdecl *)())Scr_GetFunction(pName, type);
	else
		function = (void(__cdecl *)())Scr_GetCustomFunction(pName, type);

	if (developer_funcdump->current.boolean && function != 0)
		Com_Printf(0, "[GSC] Function: %s\nType: %i\nAddress: 0x%X\n\n", *pName, *type, function);

	return function;
}

int Scr_GetMethod(int *type, const char **pName)
{
	// also aids and again do not care. #3
	int function;

	*type = 0;
	function = Player_GetMethod(pName);

	if (!function)
	{
		function = ScriptEnt_GetMethod(pName);
		if (!function)
		{
			function = ScriptVehicle_GetMethod(pName);
			if (!function)
			{
				function = HudElem_GetMethod(pName);
				if (!function)
				{
					function = Helicopter_GetMethod(pName);
					if (!function)
					{
						function = Actor_GetMethod(pName);
						if (!function)
							function = BuiltIn_GetMethod(pName, type);
					}
				}
			}
		}
	}

	if (developer_funcdump->current.boolean && function != 0)
		Com_Printf(0, "[GSC] Method: %s\nType: %i\nAddress: 0x%X\n\n", *pName, *type, function);

	return function;
}


void __declspec(naked) Scr_GetMethod_Hook(int *type, const char **pName)
{
	__asm
	{
		push esi // pName
		push edi // type
		call Scr_GetMethod
		add esp, 8
		retn
	}
}

void(__cdecl *__cdecl CScr_GetFunction_Hook(const char **pName, int *type))()
{
	// this is aids and I don't care #2
	// also if running debugger and a customf func is executed the debugger will instadie
	void(__cdecl *function)();

	function = (void(__cdecl *)())CScr_GetFunction(pName, type);

	if (developer_funcdump->current.boolean && function != 0)
		Com_Printf(0, "[CSC] Function: %s\nType: %i\nAddress: 0x%X\n\n", *pName, *type, function);

	return function;
}

void(__cdecl *__cdecl CScr_GetMethod_Hook(const char **pName, int *type))()
{
	// aids #3 woo!
	void(__cdecl *function)();

	function = (void(__cdecl *)())CScr_GetMethod(pName, type);

	if (developer_funcdump->current.boolean && function != 0)
		Com_Printf(0, "[CSC] Method: %s\nType: %i\nAddress: 0x%X\n\n", *pName, *type, function);

	return function;
}
#pragma endregion engineHKFunctions

#pragma region customFunctions
void GScr_PrintLnConsole(scr_entref_t entity)
{
	// gets amount of parameters
	if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1)
		Com_Printf(0, "^3Have one!\n");
	else
		Com_Printf(0, "^1the cake is a lie\n\n");
	// iz ded af
	//Scr_AddInt(Scr_GetNumParam(SCRIPTINSTANCE_SERVER), SCRIPTINSTANCE_SERVER);
}
#pragma endregion customFunctions

void PatchT4_Script()
{
	developer_funcdump = Dvar_RegisterBool(0, "developer_funcdump", 0, "Dump script function information (engine).");

	// [GSC]
	Detours::X86::DetourFunction((PBYTE)0x00682DAF, (PBYTE)&Scr_GetFunction_Hook, Detours::X86Option::USE_CALL);
	Detours::X86::DetourFunction((PBYTE)0x00683043, (PBYTE)&Scr_GetMethod_Hook, Detours::X86Option::USE_CALL);
	Scr_DeclareFunction("printlnconsole", GScr_PrintLnConsole);

	// [CSC]
	Detours::X86::DetourFunction((PBYTE)0x00682DC0, (PBYTE)&CScr_GetFunction_Hook, Detours::X86Option::USE_CALL);
	Detours::X86::DetourFunction((PBYTE)0x0068305C, (PBYTE)&CScr_GetMethod_Hook, Detours::X86Option::USE_CALL);

	nop(0x00465441, 2); // disable jnz on I_strnicmp for tesla notetrack

	// DON'T USE
	//nop(0x00668EDC, 5);
	//nop(0x00668F86, 5);
	//nop(0x00668E63, 5);
}