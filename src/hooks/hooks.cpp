#include "hooks.hpp"

std::deque<BaseHook*> BaseHook::hooks;

void Hooks::Init()
{
	for (auto& hook : BaseHook::GetHooks())
		hook->Init();
}

Hooks hooks;
