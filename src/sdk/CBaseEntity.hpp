#pragma once

class CBaseEntity
{
public:
	Vector GetAbsOrigin()
	{
		return getvfunc<Vector(__thiscall*)(void*)>(this, 10)(this);
	}
};
