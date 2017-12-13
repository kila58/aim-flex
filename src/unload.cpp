#include "aim-flex.hpp"

#include "unload.hpp"

#include "features/input/input.hpp"

#include "hooks/hooks.hpp"

void Unload::Poll(HMODULE module)
{
	static bool unload = false;

	input.OnKey(VK_F11, [&module]()
	{
		unload = true;
	});

	while (!unload)
		Sleep(100);

	hooks.Destroy();

	FreeLibraryAndExitThread(module, NULL);
}

Unload unload;
