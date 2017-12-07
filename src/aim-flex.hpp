#pragma once

#include <Windows.h>
#include <cctype>
#include <algorithm>
#include <deque>

#include "definitions.hpp"
#include "color.hpp"
#include "vmt.hpp"
#include "sigscan.hpp"
#include "vector.hpp"
#include "util.hpp"
#include "netvar.hpp"

#include "sdk/CEngineClient.hpp"
#include "sdk/CEngineVGui.hpp"
#include "sdk/CMatSystemSurface.hpp"
#include "sdk/CGlobalVarsBase.hpp"
#include "sdk/C_BaseEntity.hpp"
#include "sdk/IClientEntityList.hpp"
#include "sdk/CHLClient.hpp"
#include "sdk/usercmd.hpp"

extern CEngineClient* engineclient;
extern CEngineVGui* enginevgui;
extern CMatSystemSurface* matsystemsurface;
extern IClientEntityList* entitylist;
extern CHLClient* client;
extern void* clientmode;
extern CGlobalVarsBase* globals;
