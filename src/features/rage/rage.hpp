#pragma once

#include "../features.hpp"

class Rage : public BaseFeature
{
public:
	Rage() : BaseFeature() {}

	void Init();
	void Invoke();
	void Destroy();
};

extern Rage rage;
