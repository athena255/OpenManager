#pragma once
#include "pch.h"
#include "DLLInjector.h"
DWORD DLLINJECTOR_API GetProcessID(LPCWSTR procName);
uintptr_t DLLINJECTOR_API GetModuleBaseAddress(DWORD processID, LPCWSTR dllName);
DWORD DLLINJECTOR_API WaitGetProcessID(LPCWSTR exeFileName);