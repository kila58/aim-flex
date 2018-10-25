#include "WindowProc.hpp"

#include "../../features/features.hpp"

WNDPROC original_function;

LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//if (handle == GetFocus())
	{
		BaseFeature::SetArguments(WINDOWPROC, handle, msg, wparam, lparam);
		features.Invoke(WINDOWPROC);
	}

	return CallWindowProc(original_function, handle, msg, wparam, lparam);
}

void WindowProcHook::Init()
{
	original_function = (WNDPROC)SetWindowLong(FindWindow("Valve001", NULL), GWL_WNDPROC, (LONG)WindowProc);
}

void WindowProcHook::Destroy()
{
	(WNDPROC)SetWindowLong(FindWindow("Valve001", NULL), GWL_WNDPROC, (LONG)original_function);
}

WindowProcHook windowproc_hook;
