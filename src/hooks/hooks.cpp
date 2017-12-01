#include "hooks.hpp"

std::deque<BaseHook*> BaseHook::hooks;

void Hooks::Init()
{
	for (auto& i : BaseHook::GetHooks())
		i->Init();
}

Hooks hooks;
