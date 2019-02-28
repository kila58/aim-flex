#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <process.h>

#include "init.hpp"

#include <string>

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DLLInfo* info = new DLLInfo();
		info->jwt_token = "KyQBvHfGExV9a7Dn6wgrs8BJVkBuYN";// std::string((const char*)reserved);//"KyQBvHfGExV9a7Dn6wgrs8BJVkBuYN";
		info->module = module;

		DisableThreadLibraryCalls(module);
		CloseHandle((HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)Init, (LPVOID)info, NULL, NULL));
	}

	return TRUE;
}
