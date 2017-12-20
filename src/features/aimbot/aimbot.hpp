#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class C_BaseEntity;

class Aimbot : public BaseFeature
{
private:
	C_BaseEntity* lastplayer;
	C_BaseEntity* lp;
	Vector lpeyepos;
public:
	Aimbot() : BaseFeature(CREATEMOVE, 4u) {}

	void Init();
	void Invoke();
	Vector GetHitbox(C_BaseEntity* p, int hitboxindex);
	void CalculateAngle(const Vector& pos, Angle& out);
	bool IsVisible(C_BaseEntity* p, const Vector& pos);
	bool CanShoot();
	void Destroy();
};

extern Aimbot aimbot;
