// CheatManager.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "CheatManager.h"


// This is an example of an exported variable
CHEATMANAGER_API int nCheatManager=0;

// This is an example of an exported function.
CHEATMANAGER_API int fnCheatManager(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CCheatManager::CCheatManager()
{
    return;
}
