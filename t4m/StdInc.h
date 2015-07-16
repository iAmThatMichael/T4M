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
#define DATE __DATE__
#define TIME __TIME__
#define BUILDNUMBER 26
#define BUILDNUMBER_STR "26"
#define CONSOLEVERSION_STR "T4M r"BUILDNUMBER_STR"> "
#define VERSION_STR "T4-SP (r"BUILDNUMBER_STR") (built " DATE " " TIME ")"
#define BUILDLOG_STR "Build " VERSION_STR "\nlogfile created!\n"
#define SHORTVERSION_STR "2.0."BUILDNUMBER_STR
#define LONGVERSION_STR SHORTVERSION_STR " CL " DATE " " TIME