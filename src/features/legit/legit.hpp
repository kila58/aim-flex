#pragma once

#include "../features.hpp"

class Legit : public BaseFeature
{
public:
	Legit() : BaseFeature(HookType::CREATEMOVE) {}

	void Init();
	void Invoke();
	void Destroy();
};

extern Legit legit;
