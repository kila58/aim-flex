#include "Paint.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

using PaintType = void*(__thiscall*)(void*, int);
PaintType original_function;

using DrawingType = void*(__thiscall*)(void*);

void __fastcall Paint(void* instance, void*, int mode)
{
	original_function(instance, mode);

	static auto StartDrawing = (DrawingType)(SigScan("55 8B EC 83 E4 C0 83 EC 38", "vguimatsurface.dll"));
	static auto FinishDrawing = (DrawingType)(SigScan("8B 0D ? ? ? ? 56 C6 05", "vguimatsurface.dll"));

	if (mode & 1)
	{
		StartDrawing(g_MatSystemSurface);

		g_MatSystemSurface->SetDrawColor(Color(60, 186, 84, 170));
		g_MatSystemSurface->DrawFilledRect(0, 0, 8, 8);

		g_MatSystemSurface->SetDrawColor(Color(244, 194, 13, 170));
		g_MatSystemSurface->DrawFilledRect(8, 0, 8, 8);

		g_MatSystemSurface->SetDrawColor(Color(219, 50, 54, 170));
		g_MatSystemSurface->DrawFilledRect(8, 8, 8, 8);

		g_MatSystemSurface->SetDrawColor(Color(72, 133, 237, 170));
		g_MatSystemSurface->DrawFilledRect(0, 8, 8, 8);

		features.Invoke(PAINT);

		FinishDrawing(g_MatSystemSurface);
	}
}

void PaintHook::Init()
{
	hook = new Hook(g_EngineVGui, 14, &Paint);
	original_function = (PaintType)hook->ReplaceVirtual();
}

void PaintHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

PaintHook paint_hook;
