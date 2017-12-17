#pragma once

#pragma warning(disable : 4244)

#include <Windows.h>
#include <cctype>
#include <algorithm>
#include <deque>
#include <string>

#include "definitions.hpp"
#include "color.hpp"
#include "vmt.hpp"
#include "sigscan.hpp"
#include "angle.hpp"
#include "vector.hpp"
#include "util.hpp"
#include "netvar.hpp"

#include "sdk/CEngineClient.hpp"
#include "sdk/CEngineVGui.hpp"
#include "sdk/CMatSystemSurface.hpp"
#include "sdk/CGlobalVarsBase.hpp"
#include "sdk/IClientEntityList.hpp"
#include "sdk/IVModelInfo.hpp"
#include "sdk/C_BaseEntity.hpp"
#include "sdk/CHLClient.hpp"
#include "sdk/ClientModeShared.hpp"
#include "sdk/CUserCmd.hpp"
#include "sdk/C_BaseCombatWeapon.hpp"
#include "sdk/ICVar.hpp"

extern CEngineClient* engineclient;
extern CEngineVGui* enginevgui;
extern CMatSystemSurface* matsystemsurface;
extern IClientEntityList* entitylist;
extern CHLClient* client;
extern ICVar* cvar;
extern IVModelInfo* modelinfo;
extern ClientModeShared* clientmode;
extern CGlobalVarsBase* globals;
