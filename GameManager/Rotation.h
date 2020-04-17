#pragma once
// View angles
class GAMEMANAGER_API Rotation
{
	//virtual ~Rotation() = 0;
	virtual float roll() = 0;
	virtual float pitch() = 0;
	virtual float yaw() = 0;
};
