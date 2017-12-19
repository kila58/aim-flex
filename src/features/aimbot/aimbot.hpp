#pragma once

#include "../features.hpp"

class Vector;
class C_BaseEntity;

class Aimbot : public BaseFeature
{
public:
	Aimbot() : BaseFeature(CREATEMOVE, 4u) {}

	void Init();
	void Invoke();
	Vector GetHitbox(C_BaseEntity* p, int hitboxindex);
	void Destroy();
};

extern Aimbot aimbot;
