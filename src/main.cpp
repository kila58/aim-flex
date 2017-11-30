#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "init.hpp"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(module);
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Init, NULL, NULL, NULL));
	}

	return TRUE;
}
