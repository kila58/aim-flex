#include "FrameStageNotify.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"	

using FrameStageNotifyType = void*(__thiscall*)(void*, ClientFrameStage_t*);
FrameStageNotifyType original_function;

void __fastcall FrameStageNotify(void* instance, void*, ClientFrameStage_t* stage)
{
	BaseFeature::SetArguments(FRAMESTAGENOTIFY, stage);
	features.Invoke(FRAMESTAGENOTIFY);

	original_function(instance, stage);
}

void FrameStageNotifyHook::Init()
{
	hook = new Hook(client, 36, &FrameStageNotify);
	original_function = (FrameStageNotifyType)hook->ReplaceVirtual();
}

void FrameStageNotifyHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

FrameStageNotifyHook framestagenotify_hook;
