#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Hooking.h"

#include "T4.h"
#include "Utils.h"

#define __thread __declspec(thread)
#define HardDebugBreak() MessageBoxA(0, __FUNCTION__, 0, 0);
