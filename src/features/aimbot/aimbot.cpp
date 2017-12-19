#include "aimbot.hpp"

#include "../../aim-flex.hpp"

#include "../rage/rage.hpp"
#include "../legit/legit.hpp"

void Aimbot::Init()
{

}

C_BaseEntity* lastplayer;

void Aimbot::Invoke()
{
	rage.Invoke();

	lastplayer = nullptr;
}

static Vector empty = Vector(0, 0, 0);
Vector Aimbot::GetHitbox(C_BaseEntity* p, int hitboxindex)
{
	studiohdr_t* hdr = modelinfo->GetStudiomodel(p->GetModel());
	if (!hdr)
		return empty;

	mstudiobbox_t* hitbox = hdr->GetHitbox(hitboxindex, 0);
	if (!hitbox)
		return empty;

	static VMatrix bones[128];

	if (lastplayer != p)
	{
		if (!p->SetupBones(bones))
			return empty;
	}

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector min, max;
	VectorTransform(hitbox->bbmin - mod, bones[hitbox->bone], min);
	VectorTransform(hitbox->bbmax + mod, bones[hitbox->bone], max);

	lastplayer = p;

	return ((min + max) * 0.5f);
}


void Aimbot::Destroy()
{

}

Aimbot aimbot;
