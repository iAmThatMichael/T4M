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
#include "Script.h"

// custom functions
typedef struct
{
	const char* functionName;
	scr_function_t functionCall;
	int developerOnly;
} scr_funcdef_t;

static std::map<std::string, scr_funcdef_t> scriptFunctions;

void __declspec(naked) Scr_DeclareFunctionTableEntry(scr_function_t func)
{
	__asm
	{
		mov eax, 492D50h
		jmp eax
	}
}

scr_function_t Scr_GetCustomFunction(const char** name, int* isDeveloper)
{
	if (name)
	{
		if (scriptFunctions.find(std::string(*name)) != scriptFunctions.end()){
			scr_funcdef_t func = scriptFunctions[*name];

			if (func.functionName)
			{
				*name = func.functionName;
				*isDeveloper = func.developerOnly;

				return func.functionCall;
			}
		}
	}
	else
	{
		std::map<std::string, scr_funcdef_t>::iterator iter;

		for (iter = scriptFunctions.begin(); iter != scriptFunctions.end(); iter++)
		{

			scr_funcdef_t func = (*iter).second;

			Scr_DeclareFunctionTableEntry(func.functionCall);
		}
	}

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

// TODO: find GetFunction location
// Address to look at: EmitCall 00682CA0 
CallHook scrGetFunctionHook;
DWORD scrGetFunctionHookLoc = 0x44E72E;

void __declspec(naked) Scr_GetFunctionHookStub()
{
	__asm
	{
		push esi
		push edi
		call scrGetFunctionHook.pOriginal

		test eax, eax
		jnz returnToSender

		// try our own function
		call Scr_GetCustomFunction

returnToSender:
		pop edi
		pop esi

		retn
	}
}