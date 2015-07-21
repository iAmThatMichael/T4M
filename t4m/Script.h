// ==========================================================
// T4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Functionality to interact with the GameScript 
//          runtime.
//
// Initial author: NTAuthority
// Adapated: 2015-07-21
// Started: 2011-12-19
// ==========================================================

#pragma once

typedef int scr_entref_t;
typedef void(__cdecl * scr_function_t)(scr_entref_t);