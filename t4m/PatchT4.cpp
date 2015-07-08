#include "StdInc.h"

void Sys_RunInit()
{
	// Fix steam start
	//TO-DO: Find T4 addresses.
	//nop(0x86CFE3, 5); //t5sp 0x86CFE3
	//call(0x53E8D0, 0x53E8DF, PATCH_JUMP); //t5sp 0x53E8D0 0x53E8DF
}

