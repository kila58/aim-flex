#include "SceneEnd.hpp"

#include "../../features/features.hpp"

#include "../../aim-flex.hpp"

using SceneEndType = void*(__thiscall*)(void*);
SceneEndType original_function;

void __fastcall SceneEnd(void* instance, void*)
{
	features.Invoke(SCENEEND);

	original_function(instance);
}

void SceneEndHook::Init()
{
	hook = new Hook(renderview, 9, &SceneEnd);
	original_function = (SceneEndType)hook->ReplaceVirtual();
}

void SceneEndHook::Destroy()
{
	hook->RevertVirtual();

	delete hook;
}

SceneEndHook sceneend_hook;
