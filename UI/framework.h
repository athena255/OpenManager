

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <Commdlg.h>
#include <tlhelp32.h>
#include <CommCtrl.h>

#define MAX_LOADSTRING 100
#define MAX_STRLEN 260

#ifndef Assert
#define Assert(b) if (!(b)) {OutputDebugStringW(L"Assertion Failed: " #b L"\n"); return 0;}
#endif
