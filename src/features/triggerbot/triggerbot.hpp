#pragma once

#include "../features.hpp"

class Triggerbot : public BaseFeature
{
public:
	Triggerbot() : BaseFeature(CREATEMOVE, 6u) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern Triggerbot triggerbot;
