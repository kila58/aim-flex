#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class C_BaseEntity;
class CUserCmd;
class C_BaseCombatWeapon;

class Aimbot : public BaseFeature
{
private:
	C_BaseEntity* lp;
	C_BaseEntity* lastplayer;
	Vector lpeyepos;
	Angle before;
	CUserCmd* cmd;
	C_BaseCombatWeapon* weapon;
public:
	Aimbot() : BaseFeature(CREATEMOVE, 4u) {}

	void Init();
	void Invoke();
	Vector GetHitbox(C_BaseEntity* p, int index);
	void CalculateAngle(const Vector& pos, Angle& out);
	bool IsVisible(C_BaseEntity* p, const Vector& pos);
	bool CanShoot();
	void NoRecoil();
	void MovementFix();
	void End();
	void Destroy();
};

extern Aimbot aimbot;
