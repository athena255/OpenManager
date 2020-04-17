#pragma once
#include "pch.h"
#include "DLLInjector.h"
// Inject using the LoadLibrary and CreateRemoteThread method


BOOL DLLINJECTOR_API LoadLibraryInject(DWORD processID, LPCWSTR dllPath);
BOOL DLLINJECTOR_API FreeLibraryEject(DWORD processID, LPCWSTR dllPath);


