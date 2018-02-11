#include "DrawModelExecute.hpp"

#include "../../features/features.hpp"

#include "../../features/chams/chams.hpp"

#include "../../aim-flex.hpp"

using DrawModelExecuteType = void*(__thiscall*)(void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
DrawModelExecuteType original_function;

void __fastcall DrawModelExecute(void* instance, void*, void *state, const ModelRenderInfo_t& info, matrix3x4_t* bonetoworld)
{
	// todo: fix this (rendering multiple times extra from DrawModel call)

	//if (!chams.drawing)
		original_function(instance, state, info, bonetoworld);
}

void DrawModelExecuteHook::Init()
{
	hook = new Hook(modelrender, 21, &DrawModelExecute);
	original_function = (DrawModelExecuteType)hook->ReplaceVirtual();
}

void DrawModelExecuteHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

DrawModelExecuteHook drawmodelexecute_hook;
