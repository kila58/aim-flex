#pragma once

#include "../../features/playermanager/playermanager.hpp"
#include "../../netvar.hpp"

void LowerBodyYaw(const CRecvProxyData* data, void* ent, void* out)
{
	if (netvars.old_lowerbody_yaw.second)
		netvars.old_lowerbody_yaw.second(data, ent, out);

	float yaw = data->m_Value.m_Float;//std::clamp(normalize(data->m_Value.m_Float), -89.f, 89.f);
	C_BaseEntity* p = (C_BaseEntity*)ent;

	if (p && p->IsAlive())
	{
		//debug << p->GetName() << "'s lby is: " << (int)yaw << "\n";
	}
}
