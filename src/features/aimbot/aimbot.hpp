#pragma once

#include "../features.hpp"

class Vector;
class Angle;
class C_BaseEntity;

class Aimbot : public BaseFeature
{
private:
	C_BaseEntity* lastplayer;
	C_BaseEntity* lp;
public:
	Aimbot() : BaseFeature(CREATEMOVE, 4u) {}

	void Init();
	void Invoke();
	Vector GetHitbox(C_BaseEntity* p, int hitboxindex);
	void CalculateAngle(const Vector& pos, Angle& out);
	void Destroy();
};

extern Aimbot aimbot;
