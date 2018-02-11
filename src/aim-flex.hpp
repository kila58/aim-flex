#pragma once

#pragma warning(disable : 4244)

#ifndef NOMINMAX 
	#define NOMINMAX 
#endif

#include <Windows.h>
#include <cctype>
#include <algorithm>
#include <deque>
#include <array>
#include <experimental/deque>
#include <string>
#include <string_view>
#include <numeric>
#include <memory>

#include "definitions.hpp"
#include "color.hpp"
#include "vmt.hpp"
#include "sigscan.hpp"
#include "angle.hpp"
#include "vector.hpp"
#include "util.hpp"
#include "netvar.hpp"
#include "benchmark.hpp"
#include "debug.hpp"

#include "sdk/CUtlVector.hpp"
#include "sdk/IAppSystem.hpp"
#include "sdk/CEngineClient.hpp"
#include "sdk/CEngineVGui.hpp"
#include "sdk/CMatSystemSurface.hpp"
#include "sdk/CGlobalVarsBase.hpp"
#include "sdk/IClientEntityList.hpp"
#include "sdk/IVModelInfo.hpp"
#include "sdk/IPlayerAnimState.hpp"
#include "sdk/C_BaseEntity.hpp"
#include "sdk/CHLClient.hpp"
#include "sdk/ClientModeShared.hpp"
#include "sdk/CUserCmd.hpp"
#include "sdk/C_BaseCombatWeapon.hpp"
#include "sdk/ICVar.hpp"
#include "sdk/IMoveHelper.hpp"
#include "sdk/CGameMovement.hpp"
#include "sdk/IPrediction.hpp"
#include "sdk/IEngineTrace.hpp"
#include "sdk/INetChannel.hpp"
#include "sdk/IGameEvent.hpp"
#include "sdk/CInput.hpp"
#include "sdk/CClientState.hpp"
#include "sdk/IMemAlloc.hpp"
#include "sdk/IMaterialSystem.hpp"
#include "sdk/IVRenderView.hpp"
#include "sdk/IVModelRender.hpp"

extern CEngineClient* engineclient;
extern CEngineVGui* enginevgui;
extern CMatSystemSurface* matsystemsurface;
extern IClientEntityList* entitylist;
extern CHLClient* client;
extern ICVar* cvar;
extern IVModelInfo* modelinfo;
extern CGameMovement* gamemovement;
extern IPrediction* prediction;
extern IEngineTrace* enginetrace;
extern IGameEventManager2* eventmanager;
extern IMaterialSystem* materialsystem;
extern IVRenderView* renderview;
extern IVModelRender* modelrender;
extern ClientModeShared* clientmode;
extern CGlobalVarsBase* globals;
extern CInput* cmdinput;
extern CClientState* cl;
extern IMemAlloc* g_pMemAlloc;
