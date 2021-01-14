// ==========================================================
// T4M project
// 
// Component: clientdll
// Purpose: World at War patches: r_noborder for render
//          window without borders (and proper height for
//          text scaling)
//
// Initial author: NTAuthority
// Adapated: 2015-07-10
// Started: 2011-05-21
// ==========================================================

#include "StdInc.h"

StompHook windowedWindowStyleHook;
DWORD windowedWindowStyleHookLoc = 0x6D6543;

dvar_t* r_noborder;

void __declspec(naked) WindowedWindowStyleHookStub()
{
	if (r_noborder->current.value)
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
	r_noborder = Dvar_RegisterBool(0, "r_noborder", DVAR_FLAG_ARCHIVE, "Remove the border when running in windowed mode (set vid_xpos and vid_ypos to 0).");

	windowedWindowStyleHook.initialize(windowedWindowStyleHookLoc, WindowedWindowStyleHookStub, 5, false);
	windowedWindowStyleHook.installHook();
}