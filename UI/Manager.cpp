#include "Manager.h"

_Manager::_Manager() :
	gameProcessID(0),
	cheatDLL(L"") {}

_Manager& _Manager::Instance()
{
	static _Manager _Instance;
	return _Instance;
}
