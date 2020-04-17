#include "pch.h"
#include "LoadLibraryInject.h"
#include "Utility.h"

// InjectLib injects [dllPath] into [processID]
BOOL DLLINJECTOR_API LoadLibraryInject(DWORD processID, LPCWSTR dllPath)
{

	Assert(!GetModuleBaseAddress(processID, dllPath));

	HANDLE remoteProc = OpenProcess(
		PROCESS_QUERY_INFORMATION |
		PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE,
		FALSE,
		processID
	);

	Assert(remoteProc);

	size_t dllPathLen = wcslen(dllPath) * sizeof(WCHAR);

	LPWSTR remoteDllPath = (LPWSTR)VirtualAllocEx(remoteProc, NULL, dllPathLen, MEM_COMMIT, PAGE_READWRITE);
	Assert(remoteDllPath);

	Assert(WriteProcessMemory(remoteProc, remoteDllPath, dllPath, dllPathLen + 1, NULL));

	LPTHREAD_START_ROUTINE loadLibraryAddress = reinterpret_cast <LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"));
	Assert(loadLibraryAddress);

	HANDLE remoteThread = CreateRemoteThread(remoteProc, NULL, 0, loadLibraryAddress, remoteDllPath, 0, NULL);
	Assert(remoteThread);

	// Wait for remote thread to exit
	DWORD status = WaitForSingleObject(remoteThread, INFINITE);

	if (remoteDllPath && remoteProc)
		VirtualFreeEx(remoteProc, remoteDllPath, 0, MEM_RELEASE);
	if (remoteThread)
		CloseHandle(remoteThread);
	if (remoteProc)
		CloseHandle(remoteProc);

	return status == 0;
}

BOOL DLLINJECTOR_API FreeLibraryEject(DWORD processID, LPCWSTR dllPath)
{
	uintptr_t modBaseAddr = GetModuleBaseAddress(processID, dllPath);

	Assert(modBaseAddr);

	HANDLE remoteProc = OpenProcess(
		PROCESS_QUERY_INFORMATION |
		PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION,
		FALSE,
		processID
	);
	Assert(remoteProc);

	LPTHREAD_START_ROUTINE freeLibraryAddress = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary"));
	Assert(freeLibraryAddress);

	HANDLE remoteThread = CreateRemoteThread(remoteProc, NULL, 0, freeLibraryAddress, reinterpret_cast<LPVOID>(modBaseAddr), 0, NULL);
	Assert(remoteThread);

	DWORD status = WaitForSingleObject(remoteThread, INFINITE);

	if (remoteThread)
		CloseHandle(remoteThread);
	if (remoteProc)
		CloseHandle(remoteProc);

	return status == 0;
}

