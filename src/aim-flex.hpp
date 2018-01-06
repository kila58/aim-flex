#pragma once

#pragma warning(disable : 4244)

#include <Windows.h>
#include <cctype>
#include <algorithm>
#include <deque>
#include <experimental/deque>
#include <string>
#include <numeric>

#include "definitions.hpp"
#include "color.hpp"
#include "vmt.hpp"
#include "sigscan.hpp"
#include "angle.hpp"
#include "vector.hpp"
#include "util.hpp"
#include "netvar.hpp"

#include "sdk/CUtlVector.hpp"
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
extern IGameEventListener2* eventmanager;
extern ClientModeShared* clientmode;
extern CGlobalVarsBase* globals;
extern CInput* cmdinput;
