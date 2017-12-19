#pragma once

class IMoveHelper
{
public:
	void SetHost(C_BaseEntity* host)
	{
		return getvfunc<void(__thiscall*)(void*, C_BaseEntity*)>(this, 1)(this, host);
	}
};
