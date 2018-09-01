#include "DrawModelExecute.hpp"

#include "../../features/features.hpp"

#include "../../features/chams/chams.hpp"

#include "../../aim-flex.hpp"

using DrawModelExecuteType = void*(__thiscall*)(void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
DrawModelExecuteType original_function;

void __fastcall DrawModelExecute(void* instance, void*, void *state, const ModelRenderInfo_t& info, matrix3x4_t* bonetoworld)
{
	// todo: fix this (rendering multiple times extra from DrawModel call)

	//C_BaseEntity* p = (C_BaseEntity*)entitylist->GetClientEntity(info.entity_index);

	//if (!p)
		original_function(instance, state, info, bonetoworld);

	//return;
}

void DrawModelExecuteHook::Init()
{
	if (chams.mat)
	{
		hook = new Hook(modelrender, 21, &DrawModelExecute);
		original_function = (DrawModelExecuteType)hook->ReplaceVirtual();

		debug << "nibber\n";
	}
}

void DrawModelExecuteHook::Destroy()
{
	if (hook)
	{
		hook->RevertVirtual();

		delete hook;
	}
}

DrawModelExecuteHook drawmodelexecute_hook;
