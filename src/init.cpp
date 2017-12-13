#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "init.hpp"

#include "interfaces.hpp"

#include "hooks/hooks.hpp"

#include "features/features.hpp"

#include "unload.hpp"

void Init(HMODULE module)
{
	if (interfaces.Init())
	{
		hooks.Init();
		features.Init();
		unload.Poll(module);
	}
}
