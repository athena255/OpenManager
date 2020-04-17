#pragma once
#include "framework.h"

#define Manager _Manager::Instance()
// ManagerModel is a Data class shared between components of OpenManager
class _Manager
{
	_Manager();
public:
	static _Manager& Instance();
	DWORD gameProcessID;
	WCHAR cheatDLL[MAX_STRLEN];
};

