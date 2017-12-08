#pragma once

#include "../features.hpp"

class BunnyHop : public BaseFeature
{
public:
	BunnyHop() : BaseFeature(HookType::CREATEMOVE) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern BunnyHop bunnyhop;
