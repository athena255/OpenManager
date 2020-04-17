#include "pch.h"
#include "GameCSGO.h"
#include "../DLLInjector/Utility.h"
#include <stdexcept>

#define BAD_READ 69
#define BAD_WRITE 70

template<class T>
T cs_read(uintptr_t addr)
{
	static MEMORY_BASIC_INFORMATION mbi = { 0 };
	VirtualQuery(reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi));
	if (mbi.State == PAGE_NOACCESS)
	{
		__fastfail(BAD_READ);
		return 0;
	}
	return *reinterpret_cast<T*>(addr);
}

template<class T>
void cs_write(uintptr_t addr, T* thing)
{
	static MEMORY_BASIC_INFORMATION mbi = { 0 };
	VirtualQuery(reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi));
	if ( ! (mbi.Protect & PAGE_READWRITE) )
	{
		__fastfail(BAD_WRITE);
	}
	*reinterpret_cast<T*>(addr) = *thing;
}

extern CS_GameManager csgo_game;
// Vector3
float& CS_Vector3::x()
{
	return _x;
}

float& CS_Vector3::y()
{
	return _y;
}

float& CS_Vector3::z()
{
	return _z;
}

// Rotation
float CS_Rotation::roll()
{
	return _roll;
}

float CS_Rotation::pitch()
{
	return _pitch;
}

float CS_Rotation::yaw()
{
	return _yaw;
}

// Entity
bool CS_Entity::is_valid() const
{
	return true; // assume all is valid
}

bool CS_Entity::is_player() const
{
	return reinterpret_cast<CS_Player*>((uintptr_t)this)->is_alive();
}

bool CS_Entity::is_local_player() const
{
	return ((uintptr_t)csgo_game.get_local_player() == (uintptr_t)this);
}

void CS_Entity::get_location(Vector3* out_location) const
{
	Vector3* loc = cs_read<Vector3*>((uintptr_t)this + netvars::m_vecOrigin);
	cs_write((uintptr_t)out_location, loc);
}

bool CS_Entity::in_crosshairs() const
{
	int crosshair_id = cs_read<int>((uintptr_t)this + netvars::m_iCrosshairId);
	return reinterpret_cast<CS_Entity*>(csgo_game.get_entity_list()->get_entity(crosshair_id));
}

// EntityList

CEntity* CS_EntityList::first_entity() const
{
	return cs_read<CEntity*>((uintptr_t)this + 0x10);
}

CEntity* CS_EntityList::next_entity(CEntity* in_current_entity) const
{
	// TODO
	return in_current_entity;
}

size_t CS_EntityList::num_entities() const
{
	// TODO
	return 64; // assume 64 entities
}

CEntity* CS_EntityList::get_entity(size_t i)
{
	reinterpret_cast<CEntity*>((uintptr_t)this + i * 0x10);
}
// Player
bool CS_Player::is_alive() const
{
	int health = cs_read<int>((uintptr_t)this + netvars::m_iHealth);
	int alive = cs_read<int>((uintptr_t)this + netvars::m_lifeState);
	// LIFE_ALIVE = 0
	return (health > 0 && alive == 0);
}

int CS_Player::get_team() const
{
	return cs_read<int>((uintptr_t)this + netvars::m_iTeamNum);
}

bool CS_Player::is_enemy() const
{
	CS_LocalPlayer* local_player = reinterpret_cast<CS_LocalPlayer*>(csgo_game.get_local_player());
	CS_Player* this_player = reinterpret_cast<CS_Player*>((uintptr_t)this);

	return (local_player->get_team() != this_player->get_team());
}

bool CS_Player::is_visible() const
{
	return cs_read<bool>((uintptr_t)this + signatures::m_bDormant);
}

void CS_Player::get_head_location(Vector3* head_out) const
{
	uintptr_t bone_matrix = cs_read<uintptr_t>((uintptr_t)this + netvars::m_dwBoneMatrix);
	// head index is 8
	Vector3* head_pos = cs_read<Vector3*>(bone_matrix + 30 * 8 + 0xC);
	cs_write((uintptr_t)head_out, head_pos);
}

void CS_Player::get_chest_location(Vector3* chest_out) const
{
	uintptr_t bone_matrix = cs_read<uintptr_t>((uintptr_t)this + netvars::m_dwBoneMatrix);
	// chest index is 4
	Vector3* chest_pos = cs_read<Vector3*>(bone_matrix + 30 * 4 + 0xC);
	cs_write((uintptr_t)chest_out, chest_pos);
}

float CS_Player::dist_crosshairs() const
{

}

bool CS_Player::is_valid_target() const
{
	return is_alive() && is_enemy() && is_visible();
}

// LocalPlayer
void CS_LocalPlayer::set_rotation(Rotation* view_angles)
{
	
	uintptr_t old_angles = (uintptr_t)csgo_game.get_player_state() + signatures::dwClientState_ViewAngles;
	cs_write(old_angles, view_angles);
}

// GameManager
CS_GameManager::CS_GameManager() : CGameManager(L"csgo.exe")
{
	proc_id = GetProcessID(game_exe);
	engine_base = GetModuleBaseAddress(proc_id, L"engine.dll");
	client_base = GetModuleBaseAddress(proc_id, L"client_panorama.dll");
}

CEntityList* CS_GameManager::get_entity_list() const
{
	return reinterpret_cast<CEntityList*>(client_base + signatures::dwEntityList);
}

CLocalPlayer* CS_GameManager::get_local_player() const
{
	return cs_read<CLocalPlayer*>(client_base + signatures::dwLocalPlayer);
}

uintptr_t CS_GameManager::get_player_state() const
{
	return cs_read<uintptr_t>(engine_base + signatures::dwClientState);
}

bool CS_GameManager::in_game() const
{
	// 6 == FULL_CONNECTED , means we are in the server 
	return cs_read<int>((uintptr_t)get_player_state() + signatures::dwClientState_State) == 6;
}