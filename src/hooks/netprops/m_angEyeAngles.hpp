#pragma once

#include "../../features/playermanager/playermanager.hpp"
#include "../../netvar.hpp"

// pitch has autism for some reason, don't bother

void EyeAnglesPitch(const CRecvProxyData* data, void* ent, void* out)
{
	if (netvars.old_eyeangles_pitch.second)
		netvars.old_eyeangles_pitch.second(data, ent, out);

	/*
	float pitch = clamp(data->m_Value.m_Float, -89.f, 89.f);
	C_BaseEntity* p = (C_BaseEntity*)ent;

	if (p && p->IsAlive())
	{
		if (auto& player = playermanager.GetPlayer(p))
			player.resolverinfo.eye.p = pitch;
	}
	*/
}

void EyeAnglesYaw(const CRecvProxyData* data, void* ent, void* out)
{
	if (netvars.old_eyeangles_yaw.second)
		netvars.old_eyeangles_yaw.second(data, ent, out);

	/*
	float yaw = clamp(normalize(data->m_Value.m_Float), -180.f, 180.f);
	C_BaseEntity* p = (C_BaseEntity*)ent;

	if (p && p->IsAlive())
	{
		if (auto& player = playermanager.GetPlayer(p))
			player.resolverinfo.eye.y = yaw;
	}
	*/
}
