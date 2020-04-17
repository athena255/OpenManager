#pragma once

#include "resource.h"
#define UI _UI::Instance()

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// UI is Data class for Windows and Control objects
class _UI
{
public:
	HINSTANCE hInst;                                // instance handle
	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
	HWND hWndMainWindow;

	HWND hWndDLLLabel;
	HWND hWndSelectDLLButton;

	HWND hWndProcLabel;
	HWND hWndProcComboBox;

	HWND hWndInjectButton;
	HWND hWndUnloadButton;

	HWND hWndDLLLoadStatusLabel;

	static _UI& Instance();

	// Create all the windows and buttons
	void Initialize(HWND);

	BOOL HandleWMCommand(HWND, WPARAM, LPARAM);
};

VOID DisplayError();

#define ASSERT(b) do { \
	if (!(b)) {\
		DisplayError(); \
	} \
} while (0)
