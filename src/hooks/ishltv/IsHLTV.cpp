#include "IsHLTV.hpp"

#include "../../aim-flex.hpp"

using IsHLTVType = bool(__thiscall*)(void*);
IsHLTVType original_function;

bool __fastcall IsHLTV(void* instance, void*)
{
	return original_function(instance);
}

void IsHLTVHook::Init()
{
	hook = new Hook(engineclient, 93, &IsHLTV);
	original_function = (IsHLTVType)hook->ReplaceVirtual();
}

void IsHLTVHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

//IsHLTVHook ishltv_hook;
