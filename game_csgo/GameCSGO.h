#pragma once
// implements the API defined in GameManager for csgo.exe game
#include "../GameManager/GameManager.h"
#include "offsets.h"


class CS_Vector3: public Vector3
{
public:
	virtual float& x() = 0;
	virtual float& y() = 0;
	virtual float& z() = 0;
private:
	float _x;
	float _y;
	float _z;
	
};

class CS_Rotation : public Rotation
{
public:
	virtual float roll() = 0;
	virtual float pitch() = 0;
	virtual float yaw() = 0;
private:
	float _pitch;
	float _yaw;
	float _roll;
};

class CS_Entity : public CEntity
{
public:
	//virtual ~CEntity() = 0;
	// Is this entity valid?
	bool is_valid() const override;
	// Is this entity a player?
	bool is_player() const override;
	// Is this entity me?
	bool is_local_player() const override;
	// Get location of this entity
	void get_location(Vector3* out_location) const override;
	// Is this entity in my crosshairs?
	bool in_crosshairs() const override;
};

class CS_EntityList : public CEntityList
{
public:
	// Return the first entity
	CEntity* first_entity() const override;
	// Number of entities
	size_t num_entities() const override;
	// Return a pointer to the next entity
	CEntity* next_entity(CEntity* in_current_entity) const override;

	CEntity* get_entity(size_t i) override;
};

class CS_Player : public CPlayer
{
public:
	//virtual ~CPlayer() = 0;
	// Is this player an enemy?
	bool is_enemy() const;
	// Is this player a valid target (i.e. is alive? )
	bool is_valid_target() const;
	// How far is this player from our crosshairs?
	float dist_crosshairs() const;
	// Is this player even visible? (i.e. not behind a wall)
	bool is_visible() const;
	// Location of head
	void get_head_location(Vector3*) const;
	// Location of chest
	void get_chest_location(Vector3*) const;
	// Are we alive?
	bool is_alive() const override;
private:
	// Get our team number
	int get_team() const;
};

class CS_LocalPlayer : public CLocalPlayer, public CS_Player
{
public:
	// Set the rotation of this player (i.e. aim at view_angles)
	void set_rotation(Rotation* view_angles) override;

};

class CS_GameManager : public CGameManager
{
public:
	CS_GameManager();
	// Are we in game?
	bool in_game() const override;
	CEntityList* get_entity_list() const override;
	CLocalPlayer* get_local_player() const override;
	uintptr_t get_player_state() const;
	DWORD proc_id; // process id
	uintptr_t engine_base; // engine.dll
	uintptr_t client_base; // client_panorama.dll
};

CS_GameManager csgo_game;
