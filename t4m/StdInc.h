#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <zlib.h>
#pragma comment(lib, "zlib.lib")

#include "resource.h"

#include "Hooking.h"

#include "T4.h"
#include "Utils.h"

#define __thread __declspec(thread)
#define HardDebugBreak() MessageBoxA(0, __FUNCTION__, 0, 0);

//hardcoded, get a working gitrev.cmd or something
#include "buildnumber.h" // gives us COMMIT, COMMIT_STR, and BUILDHOST
#define CONSOLEVERSION_STR "T4M r"COMMIT_STR"> "
#define VERSION_STR "T4-SP (r"COMMIT_STR") (built " __DATE__ " " __TIME__ " by " BUILDHOST")"
#define SHORTVERSION_STR "2.0."COMMIT_STR