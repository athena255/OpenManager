// GameManager.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "GameManager.h"

// This is the constructor of a class that has been exported.
CGameManager::CGameManager(const wchar_t* exe_name) : 
    game_exe(exe_name), base_address (0)
{
    while (!base_address)
    {
        base_address = reinterpret_cast<uintptr_t>(GetModuleHandle(exe_name));
        Sleep(poll_rate);
    }
}
