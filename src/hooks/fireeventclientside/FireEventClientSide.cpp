#include "FireEventClientSide.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

#include "../../features/playermanager/playermanager.hpp"

using FireEventClientSideType = void*(__thiscall*)(void*, IGameEvent*);
FireEventClientSideType original_function;

void __fastcall FireEventClientSide(void* instance, void*, IGameEvent* event)
{
	if (event->GetName() == "player_death")
		playermanager.RemovePlayer(event->GetInt("userid"));

	original_function(instance, event);
}

void FireEventClientSideHook::Init()
{
	hook = new Hook(eventmanager, 9, &FireEventClientSide);
	original_function = (FireEventClientSideType)hook->ReplaceVirtual();
}

void FireEventClientSideHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

FireEventClientSideHook fireeventclientside_hook;
