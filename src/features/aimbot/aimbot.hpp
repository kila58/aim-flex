#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class C_BaseEntity;
class CUserCmd;
class C_BaseCombatWeapon;

struct Hitbox
{
	Hitbox(Vector center, int index) : center(center),  index(index) {}

	Vector center;
	int index;
};

using Hitboxes = std::deque<Hitbox>;

class Aimbot : public BaseFeature
{
private:
	C_BaseEntity* lp;
	C_BaseEntity* lastplayer;
	C_BaseEntity* lastplayer2;
	Vector lpeyepos;
	Angle before;
	CUserCmd* cmd;
	C_BaseCombatWeapon* weapon;
public:
	Aimbot() : BaseFeature(CREATEMOVE, 3u) {}

	void Init();
	void Invoke();
	Vector GetHitbox(C_BaseEntity* p, int index);
	bool GetHitboxes(C_BaseEntity* p, Hitboxes& hitboxes);
	void CalculateAngle(const Vector& pos, Angle& out);
	bool IsVisible(C_BaseEntity* p, const Vector& pos);
	void MovementFix();
	bool CanShoot();
	void NoRecoil();
	void Clamp();
	void End();
	void Destroy();
};

extern Aimbot aimbot;
