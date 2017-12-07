#include "Createmove.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"	

using CreatemoveType = void* (__thiscall*)(void*, float, CUserCmd*);
CreatemoveType original_function;

bool __fastcall Createmove(void* instance, void*, float inputSampleTime, CUserCmd* cmd)
{
	original_function(instance, inputSampleTime, cmd);

	if (!cmd->command_number)
		return true;

	features.Invoke(MOVE);

	return true;
}

void CreatemoveHook::Init()
{
	hook = new Hook(clientmode, 24, &Createmove);
	original_function = (CreatemoveType)hook->ReplaceVirtual();
}

void CreatemoveHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

CreatemoveHook paint_hook;
