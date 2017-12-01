#include "paint.hpp"

void __fastcall Paint(void* instance, void*, int mode)
{
	MessageBoxA(NULL, "in the hook", "in the hook", NULL);
}

void PaintHook::Init()
{
	hook = new Hook(g_EngineVGui, 14, &Paint);
	hook->ReplaceVirtual();
}

void PaintHook::Destory()
{
	hook->RevertVirtual();
}

PaintHook paint_hook;
