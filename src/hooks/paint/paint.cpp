#include "paint.hpp"

void __fastcall Paint(void* instance, void*, int mode)
{

}

hook* paint_hook;

void PaintHook::Init()
{
	paint_hook = new hook(*(ptr**)g_EngineVGui, 14, (ptr*)&Paint);
	paint_hook->ReplaceVirtual();
}

void PaintHook::Destory()
{
	if (paint_hook)
		paint_hook->RevertVirtual();
}
