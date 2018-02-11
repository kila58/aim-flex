#include "FireEventClientSide.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

#include "../../features/playermanager/playermanager.hpp"

using FireEventClientSideType = void*(__thiscall*)(void*, IGameEvent*);
FireEventClientSideType original_function;

void __fastcall FireEventClientSide(void* instance, void*, IGameEvent* event)
{
	std::string name = event->GetName();

	if (name == "player_death")
		playermanager.RemovePlayer(event->GetInt("userid"));
	else if (name == "player_connect")
		playermanager.AddDormantPlayer(event->GetInt("userid"));
	else if (name == "player_disconnect")
	{
		playermanager.RemovePlayer(event->GetInt("userid"));
		playermanager.RemoveDormantPlayer(event->GetInt("userid"));
	}
	else if (name == "bullet_impact")
	{
		backtrack.BulletImpact(Vector(event->GetFloat("x"), event->GetFloat("x"), event->GetFloat("x")),
			entitylist->GetClientEntity(engineclient->GetPlayerForUserID(event->GetInt("userid"))));
	}

	original_function(instance, event);
}

void FireEventClientSideHook::Init()
{
	hook = new Hook(eventmanager, 9, &FireEventClientSide);
	original_function = (FireEventClientSideType)hook->ReplaceVirtual();

	bullet_impact = new EventListener("bullet_impact");
}

void FireEventClientSideHook::Destroy()
{
	delete bullet_impact;

	hook->RevertVirtual();

	delete hook;
}

FireEventClientSideHook fireeventclientside_hook;
