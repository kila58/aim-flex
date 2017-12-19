#include "aimbot.hpp"

#include "../../aim-flex.hpp"

#include "../rage/rage.hpp"
#include "../legit/legit.hpp"

void Aimbot::Init()
{

}

void Aimbot::Invoke()
{
	rage.Invoke();
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

	VMatrix bones[128];
	if (!p->SetupBones(bones))
		return empty;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector min, max;
	VectorTransform(hitbox->bbmin - Vector(mod, mod, mod), bones[hitbox->bone], min);
	VectorTransform(hitbox->bbmax + Vector(mod, mod, mod), bones[hitbox->bone], max);

	return ((min + max) * 0.5f);
}


void Aimbot::Destroy()
{

}

Aimbot aimbot;
