#pragma once
// Local Player API
class GAMEMANAGER_API CLocalPlayer : public CEntity
{
public:
	// Set the rotation of this player (i.e. aim at view_angles)
	virtual void set_rotation(Rotation* view_angles) = 0;
};