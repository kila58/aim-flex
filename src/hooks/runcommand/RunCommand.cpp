#include "RunCommand.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"
#include "../../features/prediction/prediction.hpp"

using RunCommandType = void*(__thiscall*)(void*, C_BaseEntity*, CUserCmd*, IMoveHelper*);
RunCommandType original_function;

void __fastcall RunCommand(void* instance, void*, C_BaseEntity* player, CUserCmd* cmd, IMoveHelper* movehelper)
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

	if (player == lp)
		predict.movehelper = movehelper;

	original_function(instance, player, cmd, movehelper);
}

void RunCommandHook::Init()
{
	hook = new Hook(prediction, 19, &RunCommand);
	original_function = (RunCommandType)hook->ReplaceVirtual();
}

void RunCommandHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

RunCommandHook runcommand_hook;
