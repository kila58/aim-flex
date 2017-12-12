#include "WindowProc.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"	

WNDPROC original_function;

long __stdcall WindowProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (handle == GetFocus())
	{
		if (msg == WM_KEYDOWN && wparam == VK_F11)
		{
			MessageBoxA(NULL, "bro..", NULL, NULL);
		}
	}

	return CallWindowProc(original_function, handle, msg, wparam, lparam);
}

void WindowProcHook::Init()
{
	original_function = (WNDPROC)SetWindowLong(FindWindow("Valve001", NULL), GWL_WNDPROC, (LONG)WindowProc);
}

void WindowProcHook::Destroy()
{

}

WindowProcHook windowproc_hook;
