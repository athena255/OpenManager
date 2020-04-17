#pragma once
// Player Entity API 
class GAMEMANAGER_API CPlayer : public CEntity
{
public:
	// Is this player an enemy?
	virtual bool is_enemy() const = 0;
	// Is this player a valid target (i.e. is alive? )
	virtual bool is_valid_target() const = 0;
	// Are we alive?
	virtual bool is_alive() const = 0;
	// How far is this player from our crosshairs?
	virtual float dist_crosshairs() const = 0;
	// Is this player even visible? (i.e. not behind a wall)
	virtual bool is_visible() const = 0;
	// Location of head
	virtual void get_head_location(Vector3*) const = 0;
	// Location of chest
	virtual void get_chest_location(Vector3*) const = 0;
};