#pragma once

#include "../features.hpp"

class Rage : public BaseFeature
{
public:
	Rage() : BaseFeature(HookType::CREATEMOVE) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern Rage legit;
