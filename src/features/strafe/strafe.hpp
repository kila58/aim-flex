#pragma once

#include "../features.hpp"

class Strafe : public BaseFeature
{
public:
	Strafe() : BaseFeature(CREATEMOVE, 4u) {}

	void Init();
	void Invoke();
	void Destroy();

	bool* bsendpacket;
};

extern Strafe strafe;
