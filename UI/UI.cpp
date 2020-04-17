// UI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "UI.h"
#include "Manager.h"
#include "UIController.h"

_UI& _UI::Instance()
{
    static _UI _Instance;
    return _Instance;
}

// Create the UI Elements
void _UI::Initialize(HWND hWnd)
{
    hWndMainWindow = hWnd;

    // Make the Select Process Label
    hWndProcLabel = CreateWindow(L"Static", L"Select Process",
        WS_CHILD | WS_VISIBLE,
        100, 100, 100, 25, hWnd,
        0, hInst, 0);
    // Make the Select Process Combo Box
    hWndProcComboBox = CreateWindow(WC_COMBOBOX, L"Select Process",
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
        200, 100, 600, 500, hWnd,
        (HMENU)ID_SELECT_PROC,
        hInst, NULL);

    // Make DLL Label
    hWndDLLLabel = CreateWindow(L"Static", L"No Selected DLL",
        WS_CHILD | WS_VISIBLE,
        100, 200, 600, 25, hWnd,
        0, hInst, 0);
    // Make Select DLL Button
    hWndSelectDLLButton = CreateWindow(L"Button", L"Select DLL",
        BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
        700, 200, 100, 25, hWnd,
        (HMENU)ID_FILE_SELECTDLL,
        hInst, 0);

    // Make the DLL Inject Button
    hWndInjectButton = CreateWindow(L"Button", L"Load DLL",
        BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
        100, 400, 100, 25, hWnd,
        (HMENU)ID_FILE_LOADDLL,
        hInst, 0);
    // Make the Unload DLL Button
    hWndUnloadButton = CreateWindow(L"Button", L"Unload DLL",
        BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
        400, 400, 100, 25, hWnd,
        (HMENU)ID_FILE_UNLOADDLL,
        hInst, 0);

    // Make DLL Load Status Label
    hWndDLLLoadStatusLabel = CreateWindow(L"Static", L"DLL Not loaded",
        WS_CHILD | WS_VISIBLE,
        100, 300, 120, 25, hWnd,
        0, hInst, 0);
}

// Handle user input/actions
BOOL _UI::HandleWMCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case ID_SELECT_PROC:
        UIController.SelectProcess(hWnd, wParam, lParam);
        break;
    case ID_FILE_SELECTDLL:
        UIController.SelectDLL();
        break;
    case ID_FILE_LOADDLL:
        UIController.InjectDLL();
        break;
    case ID_FILE_UNLOADDLL:
        UIController.UnInjectDLL();
        break;
    case IDM_ABOUT:
        DialogBox(UI.hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

void DisplayError()
{
    DWORD dwErrorCode = GetLastError();

    if (!dwErrorCode)
        return;

    DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
    HLOCAL hLocal = NULL;

    BOOL bOk = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        dwErrorCode,
        systemLocale,
        (LPTSTR)&hLocal,
        0, NULL);

    if (!bOk) {
        HMODULE hDLL = LoadLibraryEx(L"netmsg.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);

        if (hDLL != NULL) {
            bOk = FormatMessage(
                FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
                FORMAT_MESSAGE_ALLOCATE_BUFFER,
                hDLL, dwErrorCode, systemLocale,
                (PWSTR)&hLocal, 0, NULL);
            FreeLibrary(hDLL);
        }
    }

    WCHAR errorBuf[MAX_STRLEN];
    if (hLocal == NULL || !bOk)
        hLocal = (HLOCAL)L"";

    wsprintf(errorBuf, L"Error Code: %d\n %s", dwErrorCode, (LPCTSTR)LocalLock(hLocal));
    MessageBox(UI.hWndMainWindow, errorBuf, L"Error", MB_OK);
    LocalFree(hLocal);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    LoadStringW(hInstance, IDS_APP_TITLE, UI.szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_UI, UI.szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UI));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_UI);
    wcex.lpszClassName = UI.szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    UI.hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindow(UI.szWindowClass, UI.szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        UI.Initialize(hWnd);
        break;
    case WM_COMMAND:
        if (!UI.HandleWMCommand(hWnd, wParam, lParam))
            return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}