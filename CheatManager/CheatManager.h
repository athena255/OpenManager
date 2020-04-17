// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the CHEATMANAGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// CHEATMANAGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHEATMANAGER_EXPORTS
#define CHEATMANAGER_API __declspec(dllexport)
#else
#define CHEATMANAGER_API __declspec(dllimport)
#endif
#include "Cheat.h"
#include "../GameManager/GameManager.h"
// This class is exported from the dll
// CheatManager is the central manager that interfaces between game structs and cheats
class CHEATMANAGER_API CCheatManager {
public:
	CCheatManager(void);
		
	// Adds a variable to var_registry
	void add_var(wchar_t* var_name, void* p_var);

	// Get variable as templated type
	template<class T>
	T* get_var(wchar_t* var_name);

	CGameManager* game_manager;

	void* enable_cheat(wchar_t* cheat_name);
	void* disable_cheat(wchar_t* cheat_name);

private:
	// used by GameManager and Cheats to share variables
	std::map<wchar_t*, void*> var_registry;
};

class CHEATMANAGER_API CCheatComponent
{
public:
	CCheatComponent(wchar_t* cheat_name);

private:
	void* enable();
	void* disable();
};

extern CHEATMANAGER_API int nCheatManager;

CHEATMANAGER_API int fnCheatManager(void);
