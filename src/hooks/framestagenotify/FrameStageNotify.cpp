#include "FrameStageNotify.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"	

#include "../../features/antiaim/antiaim.hpp"
#include "../../features/resolver/resolver.hpp"

using FrameStageNotifyType = void*(__thiscall*)(void*, ClientFrameStage_t);
FrameStageNotifyType original_function;

void __fastcall FrameStageNotify(void* instance, void*, ClientFrameStage_t stage)
{
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		BaseFeature::SetArguments(FRAMESTAGENOTIFY, stage);
		features.Invoke(FRAMESTAGENOTIFY);

		resolver.Invoke();
	}
	else if (stage == FRAME_RENDER_START)
	{
		antiaim.SetThirdPersonAngle();
	}

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
