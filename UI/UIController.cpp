#include "framework.h"
#include "UI.h"
#include "UIController.h"
#include "Manager.h"
#include "../DLLInjector/DLLInjector.h"

_UIController& _UIController::Instance()
{
    static _UIController _Instance;
	return _Instance;
}


void _UIController::SelectDLL()
{
    OPENFILENAME ofn = { 0 };
    WCHAR szFile[MAX_STRLEN] = { 0 };
    WCHAR fileTypes[] = L"DLL\0 *.DLL\0";
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = UI.hWndMainWindow;
    ofn.lpstrFile = szFile;

    wcscpy_s(ofn.lpstrFile, MAX_STRLEN, Manager.cheatDLL);
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = fileTypes;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn))
        wcscpy_s(Manager.cheatDLL, MAX_STRLEN, ofn.lpstrFile);
    SetWindowText(UI.hWndDLLLabel, ofn.lpstrFile);
}

void _UIController::PopulateProcessList(HWND hParent)
{
    SendMessage(UI.hWndProcComboBox, CB_RESETCONTENT, 0, 0);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ASSERT((snapshot != INVALID_HANDLE_VALUE));

    PROCESSENTRY32 pe{ 0 };
    pe.dwSize = sizeof(pe);
    WCHAR buf[MAX_STRLEN] = { 0 };
    LPCWSTR fmtItem = L"%s | %d";
    for (BOOL hasNext = Process32First(snapshot, &pe); hasNext; hasNext = Process32Next(snapshot, &pe))
    {
        wsprintf(buf, fmtItem, pe.szExeFile, pe.th32ProcessID);
        SendMessage(UI.hWndProcComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
    }
    if (snapshot)
        CloseHandle(snapshot);
    
}

void _UIController::SelectProcess(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

    switch (HIWORD(wParam))
    {
    case CBN_SELENDOK:
        SetGameProcess((HWND)lParam);
        break;
    case CBN_DROPDOWN:
        PopulateProcessList(hWnd);
        break;
    }
}

void _UIController::SetGameProcess(HWND hWnd)
{
    // Get the Index of the selected item
    int ItemIndex = SendMessage(hWnd, (UINT)CB_GETCURSEL, 0, 0);
    WCHAR selectedItem[MAX_STRLEN] = { 0 };
    // Get the item
    SendMessage(hWnd, CB_GETLBTEXT, ItemIndex, (LPARAM)selectedItem);
    // Display in messagebox
    DWORD pID = 0;
    wchar_t* pid;
    wcstok_s(selectedItem, L"|", &pid);
    Manager.gameProcessID = _wtoi(pid);
    ASSERT(Manager.gameProcessID != 0);
}

void _UIController::InjectDLL()
{
    if (!LoadLibraryInject(Manager.gameProcessID, Manager.cheatDLL))
        DisplayError();
    else
        SetWindowText(UI.hWndDLLLoadStatusLabel, L"DLL Loaded");
}

void _UIController::UnInjectDLL()
{
    if (!FreeLibraryEject(Manager.gameProcessID, Manager.cheatDLL))
        DisplayError();
    else
        SetWindowText(UI.hWndDLLLoadStatusLabel, L"DLL Not loaded");
}