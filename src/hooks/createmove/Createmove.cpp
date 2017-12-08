#include "CreateMove.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"	

using CreateMoveType = void*(__thiscall*)(void*, float, CUserCmd*);
CreateMoveType original_function;

bool __fastcall CreateMove(void* instance, void*, float flInputSampleTime, CUserCmd* cmd)
{
	original_function(instance, flInputSampleTime, cmd);

	if (!cmd->command_number)
		return true;

	features.Invoke(CREATEMOVE);

	return true;
}

void CreateMoveHook::Init()
{
	hook = new Hook(clientmode, 24, &CreateMove);
	original_function = (CreateMoveType)hook->ReplaceVirtual();
}

void CreateMoveHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

CreateMoveHook createmove_hook;
