#include "StdInc.h"
#include "T4.h"

StompHook windowedWindowStyleHook;
DWORD windowedWindowStyleHookLoc = 0x6D6543;

dvar_t* r_noborder;

void __declspec(naked) WindowedWindowStyleHookStub()
{
	if (r_noborder->current.boolean)
	{
		__asm mov ebp, WS_VISIBLE | WS_POPUP
	}
	else
	{
		__asm mov ebp, WS_VISIBLE | WS_SYSMENU | WS_CAPTION
	}

	__asm retn
}

void PatchT4_NoBorder()
{
	r_noborder = Dvar_RegisterBool(0, "r_noborder", DVAR_FLAG_SAVED, "Do not use a border in windowed mode");

	windowedWindowStyleHook.initialize(windowedWindowStyleHookLoc, WindowedWindowStyleHookStub, 5, false);
	windowedWindowStyleHook.installHook();
}