#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

#include <array>

class C_BaseEntity;
class CUserCmd;
class C_BaseCombatWeapon;
struct mstudiobbox_t;
struct Tick;

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
	mstudiobbox_t* hitbox;
	VMatrix bones_interp[128];
	alignas(16) VMatrix bones_uninterp[128];
	Vector mins;
	Vector maxs;
	Vector minsnoradius;
	Vector maxsnoradius;
	int hitboxindex;
public:
	Aimbot() : BaseFeature(CREATEMOVE, 5u) {}

	void Init();
	bool SetupBones(C_BaseEntity* p, int bonemask, VMatrix* bones);
	bool HitChance(C_BaseEntity* target, const Angle& ang);
	void Invoke();
	Vector GetHitbox(C_BaseEntity* p, int index, bool interpolated = true);
	float GetRadius();
	int GetHitboxIndex();
	void GetMatrix(bool interpolated, VMatrix* matrix);
	void GetHitboxBounds(Vector& mins, Vector& maxs);
	void GetHitboxBoundsNoRadius(Vector & mins, Vector & maxs);
	Vector GetBodyAim(C_BaseEntity* p);
	bool GetHitboxes(C_BaseEntity* p, Hitboxes& hitboxes, std::deque<int> selected = {});
	bool MultiPoint(C_BaseEntity* p, Tick& tick, Vector& out);
	bool MultiPoint(C_BaseEntity* p, Vector& out);
	bool MultiPoint(C_BaseEntity* p, int index, bool interpolated, Vector& out);
	void CalculateAngle(const Vector& pos, Angle& out);
	bool IsVisible(C_BaseEntity* p, const Vector& pos, int tracetype = 0, bool checkfraction = true);
	void MovementFix();
	bool CanShoot();
	void NoRecoil();
	void Clamp();
	void End();
	void Destroy();

	C_BaseEntity* target;
	Tick* tick;
	bool* bsendpacket;
};

extern Aimbot aimbot;
