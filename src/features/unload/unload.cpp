#include "unload.hpp"

#include "../../aim-flex.hpp"

#include "../../hooks/hooks.hpp"

#include "../../util.hpp"

HMODULE module;

void Unload::Init()
{
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)WorldToScreen, &(HMODULE)module);
}

void Unload::Invoke()
{
	auto arguments = GetArguments(WINDOWPROC);
	auto msg = GetArg<UINT>(arguments, 1);
	auto wparam = GetArg<WPARAM>(arguments, 2);

	if (msg == WM_KEYDOWN && wparam == VK_DELETE)
	{
		hooks.Destroy();
		::features.Destroy();
	}
}

void Unload::Destroy()
{
	FreeLibraryAndExitThread(module, 0);
}

Unload unload;
