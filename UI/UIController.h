#pragma once
#define UIController _UIController::Instance()

// UIController operates on UI and modifies Manager
class _UIController
{
	void SetGameProcess(HWND);
public:
	static _UIController& Instance();

	void SelectDLL();
	// Updates the Select Process ComboBox
	void PopulateProcessList(HWND);
	void SelectProcess(HWND, WPARAM, LPARAM);

	void InjectDLL();
	void UnInjectDLL();

};
