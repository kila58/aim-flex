#include "CreateMove.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

#include "../../features/strafe/strafe.hpp"

#include "../../features/aimbot/aimbot.hpp"

using CreateMoveType = void*(__thiscall*)(void*, float, CUserCmd*);
CreateMoveType original_function;

bool __fastcall CreateMove(void* instance, void*, float flInputSampleTime, CUserCmd* cmd)
{
	original_function(instance, flInputSampleTime, cmd);

	if (!cmd->command_number)
		return true;

	aimbot.bsendpacket = (bool*)(*GetEBP() - 0x1C);
	strafe.bsendpacket = (bool*)(*GetEBP() - 0x1C);

	// arg 1 (#2) is hook ret
	BaseFeature::SetArguments(CREATEMOVE, cmd, true, flInputSampleTime);
	features.Invoke(CREATEMOVE);

	//debug << "CreateMove cmd->move.x: " << (int)cmd->move.x << "\n";
	//debug << "CreateMove cmd->move.y: " << (int)cmd->move.y << "\n";

	return BaseFeature::GetArg<bool>(BaseFeature::GetArguments(CREATEMOVE), 1);;
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
