#pragma once

#include "../features.hpp"

class BunnyHop : public BaseFeature
{
public:
	BunnyHop() : BaseFeature(CREATEMOVE, 1u) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern BunnyHop bunnyhop;
