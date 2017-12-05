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

#include "sdk/CEngineClient.hpp"
#include "sdk/CEngineVGui.hpp"
#include "sdk/CMatSystemSurface.hpp"
#include "sdk/CGlobalVarsBase.hpp"
#include "sdk/CBaseEntity.hpp"
#include "sdk/IClientEntityList.hpp"
#include "sdk/CHLClient.hpp"

extern CEngineClient* engineclient;
extern CEngineVGui* enginevgui;
extern CMatSystemSurface* matsystemsurface;
extern IClientEntityList* entitylist;
extern CHLClient* client;
extern CGlobalVarsBase* globals;
