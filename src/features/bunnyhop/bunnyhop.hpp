#pragma once

#include "../features.hpp"

class BunnyHop : public BaseFeature
{
public:
	BunnyHop() : BaseFeature(HookType::MOVE) {}

	void Init();
	void Invoke();
	void Destroy();
};

BunnyHop bunnyhop;
