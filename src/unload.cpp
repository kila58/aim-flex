#include "aim-flex.hpp"

#include "init.hpp"

#include "unload.hpp"

#include "features/input/input.hpp"

#include "hooks/hooks.hpp"

void Unload::Poll(DLLInfo* info, bool force)
{
	if (!force)
	{
		while (!input.KeyDown(VK_F11))
			Sleep(100);
	}

	netvars.Destroy();
	hooks.Destroy();
	features.Destroy();

	Sleep(100);
	
	HMODULE module = info->module;
	delete info;

	FreeLibraryAndExitThread(module, NULL);
}

Unload unload;
