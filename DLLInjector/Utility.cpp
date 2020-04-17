#include "pch.h"
#include "DLLInjector.h"
#include <tlhelp32.h>


// Return module base address if dllName is loaded given the dllname or full path to DLL
uintptr_t DLLINJECTOR_API GetModuleBaseAddress(DWORD processID, LPCWSTR dllName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processID);
	Assert(snapshot != INVALID_HANDLE_VALUE);

	// Make sure remote process has actually loaded the target DLL
	MODULEENTRY32 me = { 0 };
	me.dwSize = sizeof(me);
	uintptr_t modBaseAddr = 0;

	for (BOOL hasNext = Module32First(snapshot, &me); hasNext && modBaseAddr == 0; hasNext = Module32Next(snapshot, &me))
	{
		if (_wcsicmp(me.szModule, dllName) == 0 || _wcsicmp(me.szExePath, dllName) == 0)
			modBaseAddr = (uintptr_t)me.modBaseAddr;
	}

	if (snapshot)
		CloseHandle(snapshot);

	return modBaseAddr;
}

// Return the process ID of the process given the process' exe filename (i.e. chrome.exe)
DWORD DLLINJECTOR_API GetProcessID(LPCWSTR exeFileName)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Assert(snapshot != INVALID_HANDLE_VALUE);

	PROCESSENTRY32 pe{ 0 };
	pe.dwSize = sizeof(pe);
	DWORD procID = 0;
	for (BOOL hasNext = Process32First(snapshot, &pe); hasNext && procID == 0; hasNext = Process32Next(snapshot, &pe))
	{
		if (_wcsicmp(pe.szExeFile, exeFileName) == 0)
			procID = pe.th32ProcessID;
	}

	if (snapshot)
		CloseHandle(snapshot);

	return procID;
}

// Keeps searching for the exeFileName in the process list
// Will not return until it finds a process with the matching exeFileName
DWORD DLLINJECTOR_API WaitGetProcessID(LPCWSTR exeFileName)
{
	HANDLE snapshot = 0;

	PROCESSENTRY32 pe{ 0 };
	pe.dwSize = sizeof(pe);
	DWORD procID = 0;
	do
	{
		if (_wcsicmp(pe.szExeFile, exeFileName) == 0)
			procID = pe.th32ProcessID;

		// If we've reached the end of the process list, get a new snapshot
		if (!Process32Next(snapshot, &pe))
		{
			snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			Assert(snapshot != INVALID_HANDLE_VALUE);

			Process32First(snapshot, &pe);
		}
	} while (procID == 0);

	if (snapshot)
		CloseHandle(snapshot);

	return procID;
}