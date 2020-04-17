#pragma once

// Cheat API represents an actual Cheat (i.e. Aimbot, ESP)
class CHEATMANAGER_API CCheat
{
public:
	CCheat(wchar_t* cheat_name) : 
		_cheat_name(cheat_name), 
		_is_enabled(0) {}

	// Do we have all the game variables needed for this cheat to work?
	virtual bool check() const = 0;

	inline void enable()
	{
		_is_enabled = 1;
	}

	inline void disable()
	{
		_is_enabled = 0;
	}

private:
	wchar_t* _cheat_name;
	bool _is_enabled;
};