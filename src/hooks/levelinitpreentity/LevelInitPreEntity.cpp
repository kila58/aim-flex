#include "LevelInitPreEntity.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

#include "../../features/playermanager/playermanager.hpp"

using LevelInitPreEntityType = void*(__thiscall*)(void*, const char* newmap);
LevelInitPreEntityType original_function;

void __fastcall LevelInitPreEntity(void* instance, void*, const char* newmap)
{
	playermanager.ClearPlayers();

	original_function(instance, newmap);
}

void LevelInitPreEntityHook::Init()
{
	hook = new Hook(client, 5, &LevelInitPreEntity);
	original_function = (LevelInitPreEntityType)hook->ReplaceVirtual();
}

void LevelInitPreEntityHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

LevelInitPreEntityHook levelinitpreentity_hook;
