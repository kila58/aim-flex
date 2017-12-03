#pragma once

#include <Windows.h>
#include <cctype>
#include <algorithm>
#include <deque>

#include "definitions.hpp"
#include "color.hpp"
#include "vmt.hpp"
#include "sigscan.hpp"

#include "sdk/CEngineClient.hpp"
#include "sdk/CEngineVGui.hpp"
#include "sdk/CMatSystemSurface.hpp"

extern CEngineClient* g_EngineClient;
extern CEngineVGui* g_EngineVGui;
extern CMatSystemSurface* g_MatSystemSurface;
