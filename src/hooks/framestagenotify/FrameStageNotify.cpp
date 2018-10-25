#include "FrameStageNotify.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"	

#include "../../features/antiaim/antiaim.hpp"
#include "../../features/resolver/resolver.hpp"
#include "../../features/backtrack/backtrack.hpp"

using FrameStageNotifyType = void*(__thiscall*)(void*, ClientFrameStage_t);
FrameStageNotifyType original_function;

void __fastcall FrameStageNotify(void* instance, void*, ClientFrameStage_t stage)
{
	BaseFeature::SetArguments(FRAMESTAGENOTIFY, (int)stage);
	features.Invoke(FRAMESTAGENOTIFY);
	
	if (stage == FRAME_RENDER_START)
	{
		antiaim.SetThirdPersonAngle();
		animations.PVSFix();
	}
	else if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		animations.FixAnimations();
	}

	// todo: fix this or find an alternate method, 
	// Exception thrown at 0x27B62EF6 (client_panorama.dll) in csgo.exe: 0xC0000005: Access violation reading location 0x00000258.
	// crashes in UpdateAnimationState
	//else if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	//	animations.UpdateServerAnimations();

	original_function(instance, stage);
}

void FrameStageNotifyHook::Init()
{
	hook = new Hook(client, 37, &FrameStageNotify);
	original_function = (FrameStageNotifyType)hook->ReplaceVirtual();
}

void FrameStageNotifyHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

FrameStageNotifyHook framestagenotify_hook;
