#include "CreateMove.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

#include "../../features/aimbot/aimbot.hpp"

using CreateMoveType = void*(__thiscall*)(void*, float, CUserCmd*);
CreateMoveType original_function;

bool __fastcall CreateMove(void* instance, void*, float flInputSampleTime, CUserCmd* cmd)
{
	original_function(instance, flInputSampleTime, cmd);

	if (!cmd->command_number)
		return false;

	aimbot.bsendpacket = (bool*)(*GetEBP() - 0x1C);

	BaseFeature::SetArguments(CREATEMOVE, cmd);
	features.Invoke(CREATEMOVE);

	return false;
	//return true;
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
