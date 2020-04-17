// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the GAMEMANAGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// GAMEMANAGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GAMEMANAGER_EXPORTS
#define GAMEMANAGER_API __declspec(dllexport)
#else
#define GAMEMANAGER_API __declspec(dllimport)
#endif
#include <map>
#include "Vector3.h"
#include "Rotation.h"
#include "Entity.h"
#include "EntityList.h"
#include "Player.h"
#include "LocalPlayer.h"

// GameManager is an adapter that allows a game to use cheats as defined in CheatManager
class GAMEMANAGER_API CGameManager {
public:
	CGameManager(const wchar_t* exe_name);
	// Are we in game?
	virtual bool in_game() const = 0;
	// Pointer to the entity list
	virtual CEntityList* get_entity_list() const = 0;
	virtual CLocalPlayer* get_local_player() const = 0;

protected:
	// process name of the game
	const wchar_t* game_exe;
	// module base address of the game process
	uintptr_t base_address;
	// default poll rate
	static constexpr unsigned int poll_rate = 100;
};

