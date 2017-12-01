#include "Paint.hpp"

#include "../../aim-flex.hpp"

using PaintType = void*(__thiscall*)(void*, int);
PaintType original_function;

void __fastcall Paint(void* instance, void*, int mode)
{
	original_function(instance, mode);

	if (mode & 1)
	{
		g_MatSystemSurface->SetDrawColor(Color(128, 128, 128));
		g_MatSystemSurface->DrawFilledRect(0, 0, 8, 8);
	}
}

void PaintHook::Init()
{
	hook = new Hook(g_EngineVGui, 14, &Paint);
	original_function = (PaintType)hook->ReplaceVirtual();
}

void PaintHook::Destory()
{
	hook->RevertVirtual();
}

PaintHook paint_hook;
