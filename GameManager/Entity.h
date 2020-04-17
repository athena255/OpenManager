#pragma once
// Entity API that game must implement
class GAMEMANAGER_API CEntity {
public:
	//virtual ~CEntity() = 0;
	// Is this entity valid?
	virtual bool is_valid() const = 0;
	// Is this entity a player?
	virtual bool is_player() const = 0;
	// Is this entity me?
	virtual bool is_local_player() const = 0;
	// Get location of this entity
	virtual void get_location(Vector3* out_location) const = 0;
	virtual bool in_crosshairs() const = 0;
};