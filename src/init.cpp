#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "init.hpp"

#include "interfaces.hpp"

#include "hooks/hooks.hpp"

#include "features/features.hpp"
#include "features/network/network.hpp"

#include "netvar.hpp"

#include "unload.hpp"

void Init(DLLInfo* info)
{
	if (interfaces.Init() && netvars.Init())
	{
		hooks.Init();

		network.jwt_token = info->jwt_token;
		network.Init();

		features.Init();

		unload.Poll(info);
	}
	else
		unload.Poll(info, true);
}
